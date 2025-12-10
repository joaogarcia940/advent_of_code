#include "../common/disjoint_set_union.h"
#include "../common/graph.h"
#include "../common/read_file.h"
#include <ranges>
#include <algorithm>
#include <cassert>
#include <iostream>
using Number = std::size_t;
using Distance = double;

constexpr Number kNumberOfPairs{100};

struct Point
{
    Number x{}, y{}, z{};
};

using JunctionBox = GraphNode<Point, Distance>;
using Circuit = Graph<Point, Distance>;
using Points = std::vector<Point>;

inline Distance StraightLineDistance(const Point p1, const Point p2)
{
    return std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z));
}

inline Distance StraightLineDistance(const JunctionBox box1, const JunctionBox box2)
{
    return StraightLineDistance(box1.data, box2.data);
}

class FileParser
{
  public:
    explicit FileParser(const Text& text) : points_{}
    {
        for (const auto& line : text)
        {
            const auto parsed_line = line | std::views::split(',') | std::views::transform([](const auto& part) {
                                         return std::stol(std::string(part.begin(), part.end()));
                                     }) |
                                     std::ranges::to<std::vector<Number>>();
            assert(parsed_line.size() == 3);
            points_.push_back({parsed_line[0], parsed_line[1], parsed_line[2]});
        }
    }

    Points GetPoints() const { return points_; }

  private:
    Points points_{};
};

class ShortestConnections
{
    struct Edge
    {
        Number p1_id{}, p2_id{};
        Distance weight{};
    };
    using Edges = std::vector<Edge>;

  public:
    explicit ShortestConnections(Points&& points, Number N)
    {
        // construct all edges
        Edges edges{};
        for (Number i{0}; i < points.size(); i++)
        {
            for (Number j{i + 1}; j < points.size(); j++)
            {
                edges.push_back({i, j, StraightLineDistance(points[i], points[j])});
            }
        }

        // sort edges by distance
        std::sort(edges.begin(), edges.end(), [](const auto lhs, const auto rhs) { return lhs.weight < rhs.weight; });

        // remove edges that do not matter
        N = std::min(N, edges.size());
        edges.erase(edges.begin() + N, edges.cend());

        // Kruskal-like algorithm
        DSU<Number> dsu(points.size());

        for (Number i{0}; i < N; i++)
        {
            dsu.Unite(edges[i].p1_id, edges[i].p2_id);
             for (Number j{0}; j < points.size(); j++)
             {
                dsu.FindParentAndCompress(j);
             }

            if (std::adjacent_find(dsu.ViewParents().begin(), dsu.ViewParents().end(), std::not_equal_to<Number>()) ==
                dsu.ViewParents().end())
            {
                result_part_2_ = points[edges[i].p1_id].x * points[edges[i].p2_id].x;
                break;
            }
        }

        std::vector<Number> sizes;
        sizes.reserve(N);

        const auto& parents = dsu.ViewParents();
        const auto& dsu_sizes = dsu.ViewSizes();

        for (Number i{0}; i < N; i++)
        {
            if (parents[i] == i)  // i is a root of a component
            {
                sizes.push_back(dsu_sizes[i]);
            }
        }

        // Sort descending
        std::sort(sizes.begin(), sizes.end(), std::greater<Number>());

        // Extract top 3
        Number first = sizes[0];
        Number second = sizes[1];
        Number third = sizes[2];

        result_part_1_ = first * second * third;
    }

    Number GetPart1Result() const { return result_part_1_; }
    Number GetPart2Result() const { return result_part_2_; }

  private:
    Number result_part_1_{};
    Number result_part_2_{};
};

constexpr std::pair<std::string, Number> example{"2025/day8/example.txt", 10};
constexpr std::pair<std::string, Number> real{"2025/day8/real.txt", 1000};

constexpr auto use_case = real;


inline void Run()
{
    const auto file = ReadFile(use_case.first);
    FileParser parser{file};
    const ShortestConnections part_1{parser.GetPoints(), use_case.second};  // 29406
    std::cout << "Part 1: " << part_1.GetPart1Result() << std::endl;
    const ShortestConnections part_2{parser.GetPoints(), std::numeric_limits<Number>::max()};  // 7499461416
    std::cout << "Part 2: " << part_2.GetPart2Result() << std::endl;
}