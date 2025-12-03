//
// Created by CTW00728 on 12/4/2024.
//

#ifndef ADVENT_DAY4_H
#define ADVENT_DAY4_H

#include "../utils/file_reader.h"
#include "../utils/numbers_in_string.h"
#include "../utils/print_vector.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace day4
{
constexpr std::string_view ex = "day4/example.txt";
constexpr std::string_view real = "day4/real.txt";
constexpr std::string_view test = "day4/test.txt";

using Num = long;
// ver todos os "X" e associar a eles uma posição
// ver todos os "M" e associar a eles uma posição
// fazer uma função a ver se são adjacentes
// se sim fazer uma lista de objectos
// no fim ver o seu tamanho

bool IsLetterValid(char letter)
{
    return letter == 'X' || letter == 'M' || letter == 'A' || letter == 'S';
}

enum class ReadingDirection
{
    None,
    Right,
    Left,
    Up,
    Down,
    UpRight,
    UpLeft,
    DownRight,
    DownLeft
};

std::string ToString(ReadingDirection reading_direction)
{
    switch (reading_direction)
    {
        case ReadingDirection::None:
            return "None";
        case ReadingDirection::Right:
            return "Right";
        case ReadingDirection::Left:
            return "Left";
        case ReadingDirection::Up:
            return "Up";
        case ReadingDirection::Down:
            return "Down";
        case ReadingDirection::UpRight:
            return "UpRight";
        case ReadingDirection::UpLeft:
            return "UpLeft";
        case ReadingDirection::DownRight:
            return "DownRight";
        case ReadingDirection::DownLeft:
            return "DownLeft";
        default:
            std::cerr << "Wrong reading direction" << std::endl;
            return "Error";
    }
}

struct DoubleLinkedListNode
{
    char letter{};
    Num x{};
    Num y{};
    std::vector<DoubleLinkedListNode*> next_nodes{};
};

using Nodes = std::vector<DoubleLinkedListNode>;

auto node_comparison = [](const Nodes& nodes_1, const Nodes& nodes_2) -> bool {
    return nodes_1.size() > nodes_2.size();
};

class FileParser
{
  public:
    explicit FileParser(Text&& text)
    {
        long cursor_x = 0;
        long cursor_y = 0;
        Nodes nodes{};
        for (auto& line : text)
        {
            cursor_x = 0;
            for (auto& letter : line)
            {
                if (IsLetterValid(letter))
                {
                    nodes.emplace_back(DoubleLinkedListNode{letter, cursor_x, cursor_y, {}});
                }
                ++cursor_x;
            }
            ++cursor_y;
        }
        nodes_ = std::move(nodes);
    }

    Nodes GetNodes() { return nodes_; }

  private:
    Nodes nodes_;
};

class Graph
{
  public:
    explicit Graph(Nodes&& nodes)
        : x_nodes_{ExtractNodesByLetter(nodes, 'X')},
          m_nodes_{ExtractNodesByLetter(nodes, 'M')},
          a_nodes_{ExtractNodesByLetter(nodes, 'A')},
          s_nodes_{ExtractNodesByLetter(nodes, 'S')},
          part_1_count_{0},
          part_2_count_{0}
    {
        std::vector<ReadingDirection> all_reading_directions = {ReadingDirection::Right,
                                                                ReadingDirection::Left,
                                                                ReadingDirection::Up,
                                                                ReadingDirection::Down,
                                                                ReadingDirection::UpRight,
                                                                ReadingDirection::UpLeft,
                                                                ReadingDirection::DownRight,
                                                                ReadingDirection::DownLeft};

        std::vector<ReadingDirection> crossed_reading_directions = {ReadingDirection::UpRight,
                                                                    ReadingDirection::UpLeft,
                                                                    ReadingDirection::DownRight,
                                                                    ReadingDirection::DownLeft};

        for (auto reading_direction : all_reading_directions)
        {
            InsertConnectionsBetween(x_nodes_, m_nodes_, reading_direction);
            InsertConnectionsBetween(m_nodes_, a_nodes_, reading_direction);
            InsertConnectionsBetween(a_nodes_, s_nodes_, reading_direction);
            long found = FindPart1CompletePathsFrom(x_nodes_);
            if (found > 0)
            {
                std::cout << ToString(reading_direction) << std::endl;
            }
            part_1_count_ += found;
            ClearGraph();
        }

        for (auto& a_node : a_nodes_)
        {
            for (auto reading_direction : crossed_reading_directions)
            {
                InsertConnectionsBetween(a_node, m_nodes_, reading_direction);
                InsertConnectionsBetween(a_node, s_nodes_, reading_direction);
            }
            long found = FindPart2XPathFrom(a_node);
            if (found >= 2)
            {
                std::cout << "found: " << found << " -> increasing counter" << std::endl;
                ++part_2_count_;
            }
            ClearGraph();
        }
    }

    long GetPart1Count() { return part_1_count_; }

    long GetPart2Count() { return part_2_count_; }

  private:
    static Nodes ExtractNodesByLetter(const Nodes& nodes, const char letter)
    {
        Nodes result{};
        for (auto&& node : nodes)
        {
            if (!IsLetterValid(node.letter) || !IsLetterValid(letter))
            {
                std::cerr << "Invalid letter" << std::endl;
            }
            if (node.letter == letter)
            {
                result.emplace_back(node);
            }
        }
        return result;
    }

    static bool IsLetterOneConnectedToLetterTwo(const char letter_1, const char letter_2)
    {
        switch (letter_1)
        {
            case 'X':
                return letter_2 == 'M';
            case 'M':
                return letter_2 == 'A';
            case 'A':
                return letter_2 == 'S';
            case 'S':
                return false;
            default:
                std::cerr << "Invalid letter" << std::endl;
        }
        return false;
    }

    static ReadingDirection GetReadingDirection(const DoubleLinkedListNode& node_1, const DoubleLinkedListNode& node_2)
    {
        auto diff_x = node_2.x - node_1.x;
        auto diff_y = node_2.y - node_1.y;
        if (diff_x == 1 && diff_y == 0)
        {
            return ReadingDirection::Right;
        }
        if (diff_x == -1 && diff_y == 0)
        {
            return ReadingDirection::Left;
        }
        if (diff_x == 0 && diff_y == -1)
        {
            return ReadingDirection::Up;
        }
        if (diff_x == 0 && diff_y == 1)
        {
            return ReadingDirection::Down;
        }
        if (diff_x == 1 && diff_y == -1)
        {
            return ReadingDirection::UpRight;
        }
        if (diff_x == -1 && diff_y == -1)
        {
            return ReadingDirection::UpLeft;
        }
        if (diff_x == 1 && diff_y == 1)
        {
            return ReadingDirection::DownRight;
        }
        if (diff_x == -1 && diff_y == 1)
        {
            return ReadingDirection::DownLeft;
        }
        return ReadingDirection::None;
    }

    static bool IsNodeOneConnectedToNodeTwo(const DoubleLinkedListNode& node_1,
                                            const DoubleLinkedListNode& node_2,
                                            ReadingDirection reading_direction)
    {
        bool return_value = GetReadingDirection(node_1, node_2) == reading_direction;
        return return_value;
    }

    static void InsertConnectionsBetween(Nodes& nodes_1, Nodes& nodes_2, ReadingDirection reading_direction)
    {
        for (auto& node_1 : nodes_1)
        {
            for (auto& node_2 : nodes_2)
            {
                if (IsNodeOneConnectedToNodeTwo(node_1, node_2, reading_direction))
                {
                    node_1.next_nodes.push_back(&node_2);
                }
            }
        }
    }

    static void InsertConnectionsBetween(DoubleLinkedListNode& node_1,
                                         Nodes& nodes_2,
                                         ReadingDirection reading_direction)
    {
        for (auto& node_2 : nodes_2)
        {
            if (IsNodeOneConnectedToNodeTwo(node_1, node_2, reading_direction))
            {
                node_1.next_nodes.push_back(&node_2);
            }
        }
    }

    long FindPart1CompletePathsFrom(const Nodes& origin_nodes)
    {
        long counter = 0;
        for (auto& origin_node : origin_nodes)  // x
        {
            if (origin_node.next_nodes.empty())
            {
                continue;
            }
            for (auto next_node_m : origin_node.next_nodes)  // m
            {
                if (next_node_m->next_nodes.empty())
                {
                    continue;
                }
                for (auto next_node_a : next_node_m->next_nodes)  // a
                {
                    if (next_node_a->next_nodes.empty())
                    {
                        continue;
                    }
                    for (auto next_node_s : next_node_a->next_nodes)  // s
                    {
                        std::cout << "found path: " << origin_node.letter << "(" << origin_node.x << ","
                                  << origin_node.y << "), " << next_node_m->letter << "(" << next_node_m->x << ","
                                  << next_node_m->y << "), " << next_node_a->letter << "(" << next_node_a->x << ","
                                  << next_node_a->y << "), " << next_node_s->letter << "(" << next_node_s->x << ","
                                  << next_node_s->y << ") | ";
                        ++counter;
                    }
                }
            }
        }

        return counter;
    }

    long FindPart2XPathFrom(const DoubleLinkedListNode& a_node)
    {
        long counter = 0;
        if (a_node.next_nodes.size() < 4)
        {
            return 0;
        }
        for (auto next_node : a_node.next_nodes)
        {
            if (next_node->letter == 'M')
            {
                auto reading_direction = GetReadingDirection(*next_node, a_node);
                auto letter_s_in_the_same_direction = std::find_if(
                    a_node.next_nodes.cbegin(),
                    a_node.next_nodes.cend(),
                    [&a_node, reading_direction](const auto node) {
                        return node->letter == 'S' && GetReadingDirection(a_node, *node) == reading_direction;
                    });
                if (letter_s_in_the_same_direction != a_node.next_nodes.cend())
                {
                    ++counter;
                }
            }
        }

        return counter;
    }

    void ClearGraph()
    {
        for (auto& x_node : x_nodes_)
        {
            x_node.next_nodes.clear();
        }
        for (auto& m_node : m_nodes_)
        {
            m_node.next_nodes.clear();
        }
        for (auto& a_node : a_nodes_)
        {
            a_node.next_nodes.clear();
        }
        for (auto& s_node : s_nodes_)
        {
            s_node.next_nodes.clear();
        }
    }

    Nodes x_nodes_;
    Nodes m_nodes_;
    Nodes a_nodes_;
    Nodes s_nodes_;
    long part_1_count_;
    long part_2_count_;

    std::set<Nodes, decltype(node_comparison)> paths_{node_comparison};
};

void Run()
{
    FileReader file_reader{real};
    FileParser file_parser{file_reader.GetText()};
    Graph graph{file_parser.GetNodes()};
    std::cout << "Part 1: Number of XMAS is " << graph.GetPart1Count() << std::endl;
    std::cout << "Part 2: Number of XMAS is " << graph.GetPart2Count() << std::endl;
}

}  // namespace day4

#endif  // ADVENT_DAY4_H
