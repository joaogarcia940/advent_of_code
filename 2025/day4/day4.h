#include "../common/graph.h"
#include "../common/read_file.h"
#include <expected>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum class Part
{
    Part1,
    Part2
};

using Number = long;
struct Position
{
    Number x{};
    Number y{};
};
using Positions = std::vector<Position>;
using Weight = Positions;

struct PaperRole
{
    Position position{};
    Number num_neighbours{};
    bool is_accessed_by_forklift{};
};
using Coordinate = std::expected<PaperRole, Position>;

struct Map
{
    Number width{};
    Number height{};
    std::vector<std::vector<Coordinate>> grid{};
};

inline std::ostream& operator<<(std::ostream& os, const Map& map)
{
    for (Number y = map.height; y-- > 0;)
    {
        for (Number x = 0; x < map.width; ++x)
        {
            const auto& coordinate = map.grid[x][y];
            if (coordinate.has_value())
            {
                if (coordinate.value().is_accessed_by_forklift)
                {
                    os << "x";
                }
                else
                {
                    os << "@";
                }
            }
            else
            {
                os << ".";
            }
        }
        os << "\n";
    }
    return os;
}

class FileParser
{
  public:
    explicit FileParser(const Text& text) : map_{}
    {
        map_.height = text.size();
        map_.width = text.front().size();
        map_.grid.resize(map_.width, std::vector<Coordinate>(map_.height));
        map_.grid.resize(map_.width, std::vector<Coordinate>(map_.height));
        for (Number y = 0; y < map_.height; ++y)
        {
            const auto& line = text[map_.height - y - 1];  // Read from bottom up
            for (Number x = 0; x < map_.width; ++x)
            {
                switch (line[x])
                {
                    case '@':
                    {
                        map_.grid[x][y] = PaperRole{Position{x, y}, {}, false};
                        break;
                    }
                    case '.':
                    {
                        map_.grid[x][y] = std::unexpected(Position{x, y});
                        break;
                    }
                }
            }
        }
    }

    Map GetMap() const { return map_; }

  private:
    Map map_{};
};

class CalculateAccessesByForklift
{
  public:
    explicit CalculateAccessesByForklift(Map&& map) : map_(std::move(map))
    {
        // std::cout << "Original map: " << std::endl << map_ << std::endl << std::endl;

        accesses_counter_part_1_ = AnnotateNeighbours();
        // std::cout << "After Part 1:\n" << map_ << std::endl << std::endl;

        // std::cout << "Part 2:\n";
        auto access_counter = AnnotateNeighboursAndRemoveAccessedByForklift();
        while (access_counter > 0)
        {
            access_counter = AnnotateNeighboursAndRemoveAccessedByForklift();
        }
    }

    Map GetMap() const { return map_; }

    Number GetAccessesCounterPart1() const { return accesses_counter_part_1_; }
    Number GetAccessesCounterPart2() const { return accesses_counter_part_2_; }

  private:
    Number AnnotateNeighboursAndRemoveAccessedByForklift()
    {
        ResetNeighbourCounter();
        const auto counter = AnnotateNeighbours();
        // std::cout << "After AnnotateNeighbours:\n" << map_ << std::endl << std::endl;

        accesses_counter_part_2_ += counter;
        // std::print("Cumulative accesses by forklift (Part 2): {}\n", accesses_counter_part_2_);
        RemoveAccessedByForklift();

        return counter;
    }

    void RemoveAccessedByForklift()
    {
        for (Number x = 0; x < map_.width; ++x)
        {
            for (Number y = 0; y < map_.height; ++y)
            {
                auto& coordinate = map_.grid[x][y];
                if (!coordinate.has_value())
                {
                    continue;
                }
                if (coordinate.value().is_accessed_by_forklift)
                {
                    coordinate = std::unexpected(Position{x, y});
                }
            }
        }
    }

    void ResetNeighbourCounter()
    {
        for (Number x = 0; x < map_.width; ++x)
        {
            for (Number y = 0; y < map_.height; ++y)
            {
                auto& coordinate = map_.grid[x][y];
                if (!coordinate.has_value())
                {
                    continue;
                }
                coordinate.value().num_neighbours = 0;
            }
        }
    }

    Number AnnotateNeighbours()
    {
        Number access_counter{0};
        for (Number x = 0; x < map_.width; ++x)
        {
            for (Number y = 0; y < map_.height; ++y)
            {
                const auto coordinate = map_.grid[x][y];
                if (!coordinate.has_value())
                {
                    continue;
                }
                map_.grid[x][y] = CalculateNeighbours(coordinate.value());
                if (map_.grid[x][y].value().num_neighbours < 4)
                {
                    map_.grid[x][y].value().is_accessed_by_forklift = true;
                    ++access_counter;
                }
            }
        }
        // std::print("Accesses by forklift: {}\n", access_counter);
        return access_counter;
    }

    PaperRole CalculateNeighbours(PaperRole coordinate) const
    {
        for (const auto& neighbour_pos : GetNeighbourPositions(coordinate.position))
        {
            const auto& neighbour_coordinate = map_.grid[neighbour_pos.x][neighbour_pos.y];
            if (!neighbour_coordinate.has_value())
            {
                continue;
            }
            ++coordinate.num_neighbours;
        }

        return coordinate;
    }

    Positions GetNeighbourPositions(const Position& pos) const
    {
        Positions neighbours{};
        if (pos.x < 0 || pos.x >= map_.width || pos.y < 0 || pos.y >= map_.height)
        {
            throw std::runtime_error("Position out of bounds");
        }
        // orthogonals
        if (pos.x > 0)
        {
            neighbours.push_back(Position{pos.x - 1, pos.y});
        }
        if (pos.x < map_.width - 1)
        {
            neighbours.push_back(Position{pos.x + 1, pos.y});
        }
        if (pos.y > 0)
        {
            neighbours.push_back(Position{pos.x, pos.y - 1});
        }
        if (pos.y < map_.height - 1)
        {
            neighbours.push_back(Position{pos.x, pos.y + 1});
        }
        // diagonals
        if (pos.x > 0 && pos.y > 0)
        {
            neighbours.push_back(Position{pos.x - 1, pos.y - 1});
        }
        if (pos.x > 0 && pos.y < map_.height - 1)
        {
            neighbours.push_back(Position{pos.x - 1, pos.y + 1});
        }
        if (pos.x < map_.width - 1 && pos.y > 0)
        {
            neighbours.push_back(Position{pos.x + 1, pos.y - 1});
        }
        if (pos.x < map_.width - 1 && pos.y < map_.height - 1)
        {
            neighbours.push_back(Position{pos.x + 1, pos.y + 1});
        }

        return neighbours;
    }

    Map map_{};
    Number accesses_counter_part_1_{0};
    Number accesses_counter_part_2_{0};
};

inline int Run()
{
    const FileParser parser{ReadFile("2025/day4/real.txt")};
    CalculateAccessesByForklift calculator{parser.GetMap()};
    std::print("Part 1 - Accesses by forklift: {}\n", calculator.GetAccessesCounterPart1());
    std::print("Part 2 - Accesses by forklift: {}\n", calculator.GetAccessesCounterPart2());
    // std::cout << calculator.GetMap();
    return 0;
}