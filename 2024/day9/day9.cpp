#include "../utils/file_reader.h"
#include <cassert>
#include <utility>
#include <variant>
#include <vector>

using Number = std::size_t;
using FileToFreeSpace = std::pair<char, char>;
using DiskMap = std::vector<FileToFreeSpace>;
using Block = std::variant<Number, char>;
using BlockRepresentation = std::vector<Block>;

inline bool IsDot(const Block& b)
{
    return std::holds_alternative<char>(b) && std::get<char>(b) == '.';
}

inline bool IsNumber(const Block& b, const Number number)
{
    return std::holds_alternative<Number>(b) && std::get<Number>(b) == number;
}

std::ostream& operator<<(std::ostream& os, const DiskMap& disk_map)
{
    for (const auto& [file_size, free_space] : disk_map)
    {
        os << "File size: " << file_size << ", Free space: " << free_space << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const BlockRepresentation& block_representation)
{
    for (const auto& block : block_representation)
    {
        std::visit([&os](const auto& value) { os << value; }, block);
    }
    return os;
}

Number FindFirstOf(const BlockRepresentation& block_representation, const Block block)
{
    for (Number i{0}; i < block_representation.size(); ++i)
    {
        if (block_representation[i].index() == block.index())
        {
            return i;
        }
    }
    return block_representation.size();
}

Number FindLastNotOf(const BlockRepresentation& block_representation, const Block block)
{
    for (Number i{block_representation.size()}; i-- > 0;)
    {
        if (block_representation[i].index() != block.index())
        {
            return i;
        }
    }
    return block_representation.size();
}

Number CalculateCheckSum(const BlockRepresentation& block_representation)
{
    Number checksum{0};
    for (Number i{0}; i < block_representation.size(); ++i)
    {
        if (std::holds_alternative<Number>(block_representation[i]))
        {
            checksum += i*std::get<Number>(block_representation[i]);
        }
    }
    return checksum;
}

class FileParser
{
  public:
    FileParser(const Text& input) : block_representation_{}
    {
        assert(input.size() == 1);
        const auto& line = input.front();

        DiskMap disk_map{};
        for (Number pair_idx{0}; pair_idx < line.size(); pair_idx += 2)
        {
            if (pair_idx + 1 >= line.size())
            {
                disk_map.push_back({line[pair_idx], '0'});
                break;
            }
            disk_map.push_back({line[pair_idx], line[pair_idx + 1]});
        }

        for (Number id{0}; id < disk_map.size(); ++id)
        {
            for (Number i{0}; i < disk_map[id].first - '0'; ++i)
            {
                block_representation_.push_back(id);
            }
            for (Number i{0}; i < disk_map[id].second - '0'; ++i)
            {
                block_representation_.push_back('.');
            }
        }
    }

    BlockRepresentation GetBlockRepresentation() const { return block_representation_; }

  private:
    BlockRepresentation block_representation_{};
};

void Part1(BlockRepresentation block_representation)
{
    auto free_space_iterator = FindFirstOf(block_representation, '.');
    auto last_occupied_space = FindLastNotOf(block_representation, '.');
    while (free_space_iterator != last_occupied_space + 1)
    {
        std::swap(block_representation[free_space_iterator], block_representation[last_occupied_space]);

        free_space_iterator = FindFirstOf(block_representation, '.');
        last_occupied_space = FindLastNotOf(block_representation, '.');
    }

    printf("Part 1 - checksum is %ld\n", CalculateCheckSum(block_representation));
}

void Part2(BlockRepresentation block_representation)
{

    auto number_iterator = block_representation.rbegin();
    while (true)
    {
        const auto last_number = std::find_if_not(number_iterator, block_representation.rend(), IsDot);
        if (last_number == block_representation.rend())
        {
            break;
        }
        const auto before_last_first_number =
            std::find_if_not(last_number, block_representation.rend(), [last_number](const Block& b) {
                return IsNumber(b, std::get<Number>(*last_number));
            });
        const auto contiguous_occupied_space = std::distance(last_number, before_last_first_number);
        auto dot_iterator_begin = block_representation.begin();
        auto dot_iterator_end = std::find_if(before_last_first_number, block_representation.rend(), IsDot).base();
        while (true)
        {
            const auto first_dot = std::find_if(dot_iterator_begin, dot_iterator_end, IsDot);
            if (first_dot == dot_iterator_end)
            {
                break;
            }
            const auto after_first_last_dot = std::find_if_not(first_dot, dot_iterator_end, IsDot);
            const auto contiguous_freespace = std::distance(first_dot, after_first_last_dot);
            if (contiguous_freespace >= contiguous_occupied_space)
            {
                for (Number i{0}; i < contiguous_occupied_space; ++i)
                {
                    std::swap(*(last_number + i), *(first_dot + i));
                }
                break;
            }
            else
            {
                dot_iterator_begin = after_first_last_dot;
            }
        }
        number_iterator = before_last_first_number;
    }

    printf("Part 2 - checksum is %ld\n", CalculateCheckSum(block_representation));
}

int main()
{
    FileReader reader{"2024/day9/real.txt"};
    FileParser parser{reader.GetText()};
    const auto blocks = parser.GetBlockRepresentation();
    Part1(blocks); // 6430446922192
    Part2(blocks); // 6460170593016
    return 0;
}