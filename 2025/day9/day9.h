#include "../common/read_file.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <ostream>

using Sz = std::size_t;
using Number = long;
using Area = long long;

struct Position
{
    Number x{}, y{};
};

inline bool operator==(const Position& a, const Position& b)
{
    return a.x == b.x && a.y == b.y;
}

inline std::ostream& operator<<(std::ostream& os, const Position& pos)
{
    os << "(" << pos.x << ", " << pos.y << ")";
    return os;
}

class RedTiles
{

    using Positions = std::vector<Position>;

    struct Edge
    {
        Position from{}, to{};
        Area weight{};
    };
    using Edges = std::vector<Edge>;

    enum class Direction
    {
        None,
        Left,
        Up,
        Right,
        Down,
    };

  public:
    explicit RedTiles(Text text)
    {
        Positions red_tiles{};
        for (const auto& line : text)
        {
            const auto red_tile = (line | std::views::split(',') | std::views::transform([](const auto& part) {
                                       return std::stol(std::string(part.begin(), part.end()));
                                   }) |
                                   std::ranges::to<std::vector<Number>>());
            assert(red_tile.size() == 2);
            red_tiles.push_back(Position{red_tile.front(), red_tile.back()});
        }
        red_tiles.push_back(red_tiles[0]);  // close the cycle to be sure all edges are considered
        red_tiles.push_back(red_tiles[1]); 

        Edges edges_part_1{};
        for (Sz i{0}; i < red_tiles.size(); ++i)
        {
            for (Sz j{i + 1}; j < red_tiles.size(); ++j)
            {
                edges_part_1.push_back({red_tiles[i], red_tiles[j], AreaBetween(red_tiles[i], red_tiles[j])});
            }
        }

        std::sort(
            edges_part_1.begin(), edges_part_1.end(), [](const Edge& a, const Edge& b) { return a.weight > b.weight; });

        largest_rectangle_part_1_ = edges_part_1.front().weight;

        Edges outer_edges_part_2{};

        for (Sz i{0}; i < red_tiles.size() - 1; ++i)
        {
            outer_edges_part_2.push_back({red_tiles[i], red_tiles[i + 1], AreaBetween(red_tiles[i], red_tiles[i + 1])});
        }

        for (Sz i{0}; i < red_tiles.size() - 2; ++i)
        {
            if (IsRectangleContainedInsideShape(outer_edges_part_2[i], outer_edges_part_2[i + 1], outer_edges_part_2[i + 2]))
            {
                const Area area = AreaBetween(outer_edges_part_2[i].from, outer_edges_part_2[i + 1].to);
            }
        }

        // std::sort(edges_part_2.begin(), edges_part_2.end(), [&red_tiles](const Edge& a, const Edge& b) {
        //     return a.weight > b.weight && IsContainedInsideShape(a.from, a.to) &&
        //            IsContainedInsideShape(b.from, b.to);
        // });
    }

    Area GetLargestRectanglePart1() const { return largest_rectangle_part_1_; }

  private:
    static Area AreaBetween(Position pos_1, Position pos_2)
    {
        return static_cast<Area>((std::abs((pos_2.x - pos_1.x)) + 1) * (std::abs((pos_2.y - pos_1.y)) + 1));
    }

    static bool IsRectangleContainedInsideShape(const Edge& curr_edge,
                                                const Edge& next_edge,
                                                const Edge& next_next_edge)
    {

        const Position missing_vertex = ExtractMissingVertex(curr_edge, next_edge);
        const Direction infer_missing_vertex_direction = InferDirection(next_edge.to, missing_vertex);
        const Direction infer_next_next_edge_direction = InferDirection(next_next_edge.from, next_next_edge.to);

        return infer_missing_vertex_direction == infer_next_next_edge_direction;
    }

    // const auto vertices = ExtractVertices(curr_edge.from, curr_edge.to);
    // const std::vector<decltype(red_tiles.begin())> shape_vertices{};

    // assert(shape_vertices.size() >= 3);
    // assert(missing_vertex.size() <= 1);

    // if (missing_vertex.empty())
    // {
    //     return true;
    // }

    // // find edges
    // const std::vector<std::pair<std::pair<Sz, Edge>, Direction>> shape_edges{};
    // for (Sz i{0}; i < shape_vertices.size() - 1; ++i)
    // {
    //     const auto vertex_1 = shape_vertices[i];
    //     const auto vertex_2 = shape_vertices[i + 1];
    //     const auto shape_edge =
    //         std::find_if(edges.begin(), edges.end(), [vertex_1, vertex_2, &red_tiles](const auto& edge) {
    //             return red_tiles[edge.pos_id_1].x == vertex_1->x && red_tiles[edge.pos_id_1].y == vertex_1->y &&
    //                    red_tiles[edge.pos_id_2].x == vertex_2->x && red_tiles[edge.pos_id_2].y == vertex_2->y;
    //         });
    //     assert(shape_edge != edges.end());

    //     shape_edges.push_back({{std::distance(edges.begin(), shape_edge), *shape_edge},
    //                            InferDirection(red_tiles[shape_edge->pos_id_1],
    //                            red_tiles[shape_edge->pos_id_2])});
    // }
    // assert(shape_edges.size() >= 2);

    // std::sort(shape_edges.begin(), shape_edges.end(), [](const auto& a, const auto& b) {
    //     return a.first.first < b.first.first;
    // });

    // // reconstruct possible rectangle
    // for (Sz i{0}; i < shape_edges.size() - 1; ++i)
    // {
    //     const auto id = shape_edges[i].first.first;
    //     const auto next_id = shape_edges[i + 1].first.first;
    //     assert(next_id - id == 1);

    //     const auto direction = shape_edges[i].second;
    //     const auto next_direction = shape_edges[i + 1].second;

    //     const Position inferred_next_point{};

    //     switch (direction)
    //     {
    //         case Direction::Up:
    //             if (missing_vertex.)
    //             {

    //             }
    //             switch (next_direction)
    //             {
    //                 case Direction::Right:
    //                     break;
    //                 case Direction::Left:

    //                     break;
    //                 default:
    //                     assert(false && "Unexpected direction");
    //             }
    //             break;
    //     }
    // }

    // reconstruct possible rectangle

    // const auto direction = InferDirection(red_tiles[shape_vertices[0] - red_tiles.begin()],
    //                                       red_tiles[shape_vertices[1] - red_tiles.begin()]);
    // const auto next_direction = shape_edges[i + 1].second;

    // const Position inferred_next_point{};

    // switch (direction)
    // {
    //     case Direction::Up:
    //         if (missing_vertex.)
    //         {
    //         }
    //         switch (next_direction)
    //         {
    //             case Direction::Right:
    //                 break;
    //             case Direction::Left:

    //                 break;
    //             default:
    //                 assert(false && "Unexpected direction");
    //         }
    //         break;
    // }

    // return true;
    // }

    static Position ExtractMissingVertex(const Edge& curr_edge, const Edge& next_edge)
    {
        const auto curr_direction = InferDirection(curr_edge.from, curr_edge.to);
        const auto next_direction = InferDirection(next_edge.from, next_edge.to);

        Position missing_vertex{};
        switch (curr_direction)
        {
            case Direction::Up:
            case Direction::Down:
                return Position{curr_edge.from.y, next_edge.to.x};
            case Direction::Right:
            case Direction::Left:
                return Position{curr_edge.from.x, next_edge.to.y};
            default:
                //
                ;
        }
    }

    static Direction InferDirection(const Position from, const Position to)
    {
        if (from.x < to.x)
        {
            return Direction::Right;
        }
        else if (from.x > to.x)
        {
            return Direction::Left;
        }
        else if (from.y < to.y)
        {
            return Direction::Up;
        }
        else if (from.y > to.y)
        {
            return Direction::Down;
        }
        else
        {
            //
        }
        return Direction::None;
    }

    static Positions ExtractVertices(const Position pos_1, const Position pos_2)
    {
        const Number horizontal_distance = std::abs(pos_2.x - pos_1.x);
        const Number vertical_distance = std::abs(pos_2.y - pos_1.y);

        const Position down_left_vertex = {std::min(pos_1.x, pos_2.x), std::min(pos_1.y, pos_2.y)};
        const Position down_right_vertex = {down_left_vertex.x + horizontal_distance, down_left_vertex.y};
        const Position up_left_vertex = {down_left_vertex.x, down_left_vertex.y + vertical_distance};
        const Position up_right_vertex = {down_left_vertex.x + horizontal_distance,
                                          down_left_vertex.y + vertical_distance};
        return {down_left_vertex, down_right_vertex, up_left_vertex, up_right_vertex};
    }

    static auto FindShapeVertex(const Position pos, const auto& red_tiles)
    {
        return std::find_if(red_tiles.begin(), red_tiles.end(), [pos](const auto& red_tile) {
            return red_tile.x == pos.x && red_tile.y == pos.y;
        });
    }

    static bool IsVertexContainedInShape(const Position pos, const Edge& edge, const auto& red_tiles)
    {
        if (IsVertexAShapeVertex(pos, red_tiles))
        {
            return true;
        }

        if ()

            //
            //
            return true;
    }

    void PrintMap(const auto& red_tiles)
    {
        const auto width =
            std::max_element(
                red_tiles.begin(), red_tiles.end(), [](const Position& a, const Position& b) { return a.x < b.x; })
                ->x +
            3;
        const auto height =
            std::max_element(
                red_tiles.begin(), red_tiles.end(), [](const Position& a, const Position& b) { return a.y < b.y; })
                ->y +
            2;

        for (Sz y{}; y < height; ++y)
        {
            for (Sz x{}; x < width; ++x)
            {
                if (std::find_if(red_tiles.begin(), red_tiles.end(), [x, y](const Position& pos) {
                        return pos.x == x && pos.y == y;
                    }) != red_tiles.end())
                {
                    std::cout << "#";
                }
                else
                {
                    std::cout << ".";
                }
            }
            std::cout << "\n";
        }
    }

    Area largest_rectangle_part_1_{};
};

inline void Run()
{
    const auto red_tiles = RedTiles{ReadFile("2025/day9/example.txt")};
    std::cout << "Part 1 - " << red_tiles.GetLargestRectanglePart1() << "\n";
}