#include "../utils/file_reader.h"
#include "../utils/graph_containers.h"
#include <cstdio>
#include <ostream>
#include <unordered_map>
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
using Weight = std::vector<Position>;

struct Antenna
{
    using Id = char;
    Id id{};
    Position position{};
};

using AntennaGraph = Graph<Antenna, Weight>;
using AntennaGraphPtr = std::shared_ptr<AntennaGraph>;
using AntennaGraphNode = GraphNode<Antenna, Weight>;
using AntennaGraphNodePtr = std::shared_ptr<AntennaGraphNode>;
struct Coordinate
{
    Position position{};
    AntennaGraphNodePtr antenna_node_ptr{};
    bool is_antinode{};
};

struct Map
{
    Number width{};
    Number height{};
    std::vector<std::vector<Coordinate>> grid{};
    std::unordered_map<Antenna::Id, AntennaGraphPtr> antenna_ptr_map{};
};

std::ostream& operator<<(std::ostream& os, const Map& map)
{
    for (Number y = map.height; y-- > 0;)
    {
        for (Number x = 0; x < map.width; ++x)
        {
            const auto& coordinate = map.grid[x][y];
            if (coordinate.antenna_node_ptr != nullptr)
            {
                os << coordinate.antenna_node_ptr->data.id;
            }
            else if (coordinate.is_antinode)
            {
                os << '#';
            }
            else
            {
                os << '.';
            }
        }
        os << '\n';
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
        for (Number y = 0; y < map_.height; ++y)
        {
            const auto& line = text[map_.height - y - 1];  // Read from bottom up
            for (Number x = 0; x < map_.width; ++x)
            {
                const auto& ch = line[x];
                map_.grid[x][y].position = Position{x, y};
                if (ch != '.')
                {
                    Antenna antenna{ch, Position{x, y}};
                    const auto antenna_node_ptr = std::make_shared<AntennaGraphNode>(antenna);
                    map_.grid[x][y].antenna_node_ptr = antenna_node_ptr;

                    if (map_.antenna_ptr_map.find(ch) == map_.antenna_ptr_map.end())
                    {
                        AntennaGraphPtr antenna_graph_ptr = std::make_shared<AntennaGraph>();
                        map_.antenna_ptr_map[ch] = antenna_graph_ptr;
                    }
                    map_.antenna_ptr_map[ch]->AddNode(antenna);
                }
            }
        }
    }

    Map GetMap() const { return map_; }

  private:
    Map map_{};
};

class CalculateAntinodes
{
  public:
    explicit CalculateAntinodes(Map&& map, const Part part) : map_(std::move(map)), part_(part)
    {
        AddEdges();
        AnnotateAntinodes();
    }

    Map GetMap() const { return map_; }

    Number GetAntinodesCounter() const { return antinodes_counter_; }

  private:
    void AddEdges()
    {
        for (auto& [id, antenna_graph_ptr] : map_.antenna_ptr_map)
        {
            for (std::size_t i = 0; i < antenna_graph_ptr->nodes.size(); ++i)
            {
                const auto& node_i = antenna_graph_ptr->nodes[i];
                for (std::size_t j = 0; j < antenna_graph_ptr->nodes.size(); ++j)
                {
                    if (i == j)
                    {
                        continue;
                    }
                    const auto& node_j = antenna_graph_ptr->nodes[j];
                    switch (part_)
                    {
                        case Part::Part1:
                        {
                            antenna_graph_ptr->AddEdge(
                                node_i, node_j, {GetAntinode(node_i->data.position, node_j->data.position)});
                            break;
                        }
                        case Part::Part2:
                        {
                            std::vector<Position> antinode_positions{};
                            auto current_position = node_i->data.position;
                            auto next_position = node_j->data.position;
                            while (next_position.x >= 0 && next_position.x < map_.width && next_position.y >= 0 &&
                                   next_position.y < map_.height)
                            {
                                antinode_positions.push_back(next_position);
                                const auto next_antinode = GetAntinode(current_position, next_position);
                                current_position = next_position;
                                next_position = next_antinode;
                            }
                            antenna_graph_ptr->AddEdge(node_i, node_j, antinode_positions);
                            break;
                        }
                    }
                }
            }
        }
    }

    void AnnotateAntinodes()
    {
        for (auto& [id, antenna_graph_ptr] : map_.antenna_ptr_map)
        {
            for (const auto& node : antenna_graph_ptr->nodes)
            {
                for (const auto& edge : node->edges)
                {
                    const auto antinode_positions = edge.weight;
                    for (const auto& antinode_pos : antinode_positions)
                    {
                        if (antinode_pos.x < 0 || antinode_pos.x >= map_.width || antinode_pos.y < 0 ||
                            antinode_pos.y >= map_.height)
                        {
                            continue;
                        }
                        auto& antinode_coordinate = map_.grid[antinode_pos.x][antinode_pos.y];
                        if (antinode_coordinate.is_antinode)
                        {
                            continue;
                        }
                        antinode_coordinate.is_antinode = true;
                        ++antinodes_counter_;
                    }
                }
            }
        }
    }

    Position GetAntinode(const Position& pos1, const Position& pos2)
    {
        return {pos2.x + (pos2.x - pos1.x), pos2.y + (pos2.y - pos1.y)};
    }

    Map map_{};
    Number antinodes_counter_{0};
    Part part_;
};

int main()
{
    const FileReader reader{"2024/day8/real.txt"};
    const FileParser parser{reader.GetText()};
    CalculateAntinodes calculator_part_1{parser.GetMap(), Part::Part1};
    // std::cout << calculator_part_1.GetMap();
    std::cout << "Part 1 - Antinodes count: " << calculator_part_1.GetAntinodesCounter() << std::endl;  // 247
    CalculateAntinodes calculator_part_2{parser.GetMap(), Part::Part2};
    // std::cout << calculator_part_2.GetMap();
    std::cout << "Part 2 - Antinodes count: " << calculator_part_2.GetAntinodesCounter() << std::endl;  // 861
    return 0;
}