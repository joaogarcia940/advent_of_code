#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <tuple>
#include <unordered_set>
#include <vector>

template <typename Cargo>
class RTree2d
{
public:
    using MortonIdx = uint32_t;

    using Element = std::pair<Cargo, MortonIdx>;

    struct Point
    {
        using type = int16_t;
        type x = 0;
        type y = 0;

        static Point min() noexcept
        {
            return Point{std::numeric_limits<type>::min(),
                         std::numeric_limits<type>::min()};
        }

        static Point max() noexcept
        {
            return Point{std::numeric_limits<type>::max(),
                         std::numeric_limits<type>::max()};
        }

        Point operator-(Point p)
        {
            return Point{int16_t(this->x - p.x), int16_t(this->y - p.y)};
        }

        friend std::ostream &operator<<(std::ostream &os, const Point &p)
        {
            os << '[' << p.x << ',' << p.y << ']';
            return os;
        }
    };

    class Box
    {
    public:
        Box() = default;
        Box(Point lb, Point ub) : lb_(lb), ub_(ub)
        {
            if (lb_.x > ub_.x || lb_.y > ub_.y)
            {
                throw std::runtime_error("Bad box!");
            }
        }

        Point midpoint() const noexcept
        {
            return Point{std::midpoint(lb_.x, ub_.x), std::midpoint(lb_.y, ub_.y)};
        }

        bool contains(Point p) const noexcept
        {
            return (p.x >= lb_.x) && (p.x <= ub_.x) && (p.y >= lb_.y) &&
                   (p.y <= ub_.y);
        }

        Point lb() const noexcept { return lb_; }
        Point ub() const noexcept { return ub_; }

        friend std::ostream &operator<<(std::ostream &os, const Box &box)
        {
            os << "lb: " << box.lb() << " ub: " << box.ub();
            return os;
        }

    private:
        Point lb_ = {0};
        Point ub_ = {0};

        friend class RTree2d;
    };

    static Box bounding_box(Box a, Box b) noexcept
    {
        Point lb;
        Point ub;

        lb.x = std::min(a.lb_.x, b.lb_.x);
        lb.y = std::min(a.lb_.y, b.lb_.y);

        ub.x = std::max(a.ub_.x, b.ub_.x);
        ub.y = std::max(a.ub_.y, b.ub_.y);

        return Box(lb, ub);
    }

    RTree2d() : levels_(1) {}

    void insert(Cargo c, Box b)
    {
        cargo_.push_back(c);
        levels_.at(0).push_back(b);
    }

    void build_tree()
    {
        if (levels_[0].size() == 0)
        {
            throw std::runtime_error("No cargo inserted into the tree!");
        }

        domain_ = std::accumulate(levels_[0].begin(), levels_[0].end(),
                                  levels_[0].front(), bounding_box);

        using SortType = std::tuple<MortonIdx, Cargo, Box>;

        std::vector<SortType> sort_vec;
        sort_vec.reserve(cargo_.size());

        for (size_t i = 0; i < cargo_.size(); i++)
        {
            sort_vec.emplace_back(morton(levels_[0][i].midpoint() - domain_.lb()),
                                  cargo_[i], levels_[0][i]);
        }

        std::sort(sort_vec.begin(), sort_vec.end(),
                  [](const SortType &a, const SortType &b)
                  {
                      return std::get<0>(a) < std::get<0>(b);
                  });

        morton_.reserve(cargo_.size());

        size_t i = 0;
        for (auto &&[m, cargo, box] : sort_vec)
        {
            morton_.push_back(m);
            cargo_[i] = cargo;
            levels_[0][i] = box;
            i++;
        }

        size_t n_boxes_to_pack = levels_[0].size();

        while (n_boxes_to_pack > 1)
        {
            const auto &prev_level = levels_.back();
            auto &next_level = levels_.emplace_back();

            for (size_t i = 0; i < prev_level.size(); i += 2)
            {
                Box bb = prev_level[i];
                if (i + 1 < prev_level.size())
                {
                    bb = bounding_box(bb, prev_level.at(i + 1));
                }
                next_level.push_back(bb);
            }

            n_boxes_to_pack = next_level.size();
        }
    }

    std::vector<Element> find_cargo_at(Point p) const
    {
        size_t level = levels_.size() - 1;
        const auto &back = levels_.back();
        std::vector<Element> found;
        found.reserve(2);
        for (size_t i = 0; i < back.size(); i++)
        {
            if (back[i].contains(p))
            {
                if (level == 0)
                {
                    found.emplace_back(cargo_[i], morton_[i]);
                }
                else
                {
                    find_cargo_at_level(level - 1, i * 2, p, found);
                }
            }
        }

        return found;
    }

private:
    void find_cargo_at_level(size_t level, size_t start_idx, Point p,
                             std::vector<Element> &found) const
    {
        const auto &l = levels_[level];
        size_t end = start_idx + 2 < l.size() ? start_idx + 2 : l.size();
        for (size_t i = start_idx; i < end; i++)
        {
            if (l[i].contains(p))
            {
                if (level == 0)
                {
                    found.emplace_back(cargo_[i], morton_[i]);
                }
                else
                {
                    find_cargo_at_level(level - 1, i * 2, p, found);
                }
            }
        }
    }

    std::vector<MortonIdx> morton_;
    std::vector<Cargo> cargo_;
    std::deque<std::vector<Box>> levels_;
    Box domain_;

    MortonIdx morton(Point p) const noexcept
    {
        MortonIdx m = 0;

        for (int i = 0; i < 16; i++)
        {
            typename Point::type bx = (p.x & (1 << i)) >> i;
            typename Point::type by = (p.y & (1 << i)) >> i;
            bx <<= 2 * i;
            by <<= 2 * i + 1;
            m |= (MortonIdx)(bx | by);
        }

        return m;
    }
};

static std::pair<int64_t, size_t> parse_num(const std::string &str,
                                            size_t start)
{
    int64_t x = 0;
    size_t i = start;
    while (i < str.size() && (str[i] >= '0' && str[i] <= '9'))
    {
        x *= 10;
        x += str[i] - '0';
        i++;
    }

    return std::make_pair(x, i);
}

template <>
struct std::hash<typename RTree2d<int64_t>::Element>
{
    using Element = typename RTree2d<int64_t>::Element;
    size_t operator()(const Element &e) const
    {
        return std::hash<decltype(e.second)>()(e.second);
    }
};

int main()
{
    using RTree = RTree2d<int64_t>;
    using Point = typename RTree::Point;
    using Box = typename RTree::Box;
    using Element = typename RTree::Element;
    using SymbolPair = std::pair<char, Point>;

    RTree rtree;
    std::vector<SymbolPair> symbols;

    std::ifstream file("/home/joao/workspace/advent/day3/input.txt");

    std::string line;
    int32_t line_number = 0;
    while (std::getline(file, line))
    {
        for (size_t i = 0; i < line.size();)
        {
            if (line[i] >= '0' && line[i] <= '9')
            {
                auto [cargo, end] = parse_num(line, i);

                Point lb(int16_t(i) - 1, int16_t(line_number) - 1);
                Point ub(int16_t(end), int16_t(line_number) + 1);

                Box cargo_box(lb, ub);

                rtree.insert(cargo, cargo_box);

                i = end;
            }
            else if (line[i] != '.')
            {
                symbols.emplace_back(line[i], Point(i, line_number));
                i++;
            }
            else
            {
                i++;
            }
        }
        line_number++;
    }

    rtree.build_tree();

    std::unordered_set<Element> product_numbers;

    int64_t part2 = 0;

    for (auto &[s, p] : symbols)
    {
        auto results = rtree.find_cargo_at(p);
        for (auto &&pair : results)
        {
            product_numbers.insert(pair);
        }

        if (s == '*' && results.size() == 2)
        {
            part2 += results[0].first * results[1].first;
        }
    }

    int64_t part1 = 0;
    std::ofstream outputFile; // Declare an output file stream object
    outputFile.open("/home/joao/workspace/advent/day3/part1_sol.txt", std::ios::out); // Open "new_test.txt" in append mode
    for (auto &[pn, m] : product_numbers)
    {
        outputFile << pn << std::endl;
        part1 += pn;
    }
    outputFile.close();

    std::cout
        << "Part 1: " << part1 << '\n';
    std::cout << "Part 2: " << part2 << '\n';
}