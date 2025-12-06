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

Number FindFirstFreeSpaceOf(const BlockRepresentation& block_representation)
{
    for (Number i{0}; i < block_representation.size(); ++i)
    {
        if (std::holds_alternative<char>(block_representation[i]))
        {
            return i;
        }
    }
    return block_representation.size();
}

Number FindLastNotFreeSpaceOf(const BlockRepresentation& block_representation)
{
    for (Number i{block_representation.size()}; i-- > 0;)
    {
        if (std::holds_alternative<Number>(block_representation[i]))
        {
            return i;
        }
    }
    return block_representation.size();
}

class FileParser
{
  public:
    FileParser(const Text& input)
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

        BlockRepresentation block_representation{};
        for (Number id{0}; id < disk_map.size(); ++id)
        {
            for (Number i{0}; i < disk_map[id].first - '0'; ++i)
            {
                block_representation.push_back(id);
            }
            for (Number i{0}; i < disk_map[id].second - '0'; ++i)
            {
                block_representation.push_back('.');
            }
        }

        auto free_space_iterator = FindFirstFreeSpaceOf(block_representation);
        auto last_occupied_space = FindLastNotFreeSpaceOf(block_representation);
        while (free_space_iterator != last_occupied_space + 1)
        {
            std::swap(block_representation[free_space_iterator], block_representation[last_occupied_space]);

            free_space_iterator = FindFirstFreeSpaceOf(block_representation);
            last_occupied_space = FindLastNotFreeSpaceOf(block_representation);
        }

        Number checksum{0};
        for (Number i{0}; i <= last_occupied_space; ++i)
        {
            assert(std::holds_alternative<Number>(block_representation[i]));
            checksum += i * std::get<Number>(block_representation[i]);
        }

        printf("Part 1 - checksum is %ld\n", checksum);
    }
};

int main()
{
    FileReader reader{"2024/day9/real.txt"};
    FileParser parser{reader.GetText()};
    return 0;
}