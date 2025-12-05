#include "../common/read_file.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <unordered_set>

using Id = long;
using Ids = std::unordered_set<Id>;
using ValidIdRange = std::pair<Id, Id>;
using ValidIdRanges = std::vector<ValidIdRange>;


inline std::ostream& operator<<(std::ostream& os, const Ids& ids)
{
    for (const auto& id : ids)
    {
        os << id << " ";
    }

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ValidIdRange& ids)
{
    os << ids.first << "-" << ids.second;

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ValidIdRanges& range)
{
    for (const auto& ids : range)
    {
        os << ids << std::endl;
    }

    return os;
}


class FileParser
{
  public:
    FileParser(const Text& input)
    {
        bool reading_id_ranges = true;
        for (const auto& line : input)
        {
            if (line == "\n" || line.empty())
            {
                reading_id_ranges = false;
                continue;
            }
            if (reading_id_ranges)
            {
                valid_id_ranges_.push_back(ExtractValidIdRanges(line));
            }
            else
            {
                available_ids_.emplace(ExtractAvailableIds(line));
            }
        }
    }

    const ValidIdRanges& ViewValidIdRanges() const { return valid_id_ranges_; }

    const Ids& ViewAvailableIds() const { return available_ids_; }

  private:
    ValidIdRange ExtractValidIdRanges(const auto& line)
    {
        std::vector<Id> id_range =
            line | std::views::split('-') |
            std::views::transform([](const auto& part) { return std::stol(std::string(part.begin(), part.end())); }) |
            std::ranges::to<std::vector<Id>>();
        if (id_range.size() != 2)
        {
            throw std::runtime_error("Invalid ID format in line: " + line);
        }
        return {id_range.front(), id_range.back()};
    }

    Id ExtractAvailableIds(const auto& line) { return std::stol(line); }

    ValidIdRanges valid_id_ranges_;
    Ids available_ids_;
};

class CalculateValidIds
{
  public:
    CalculateValidIds(const ValidIdRanges& valid_id_ranges, const Ids& available_ids) : available_valid_ids_{}
    {
        available_valid_ids_ = CalculateAvailableValidIds(valid_id_ranges, available_ids);
        number_of_all_valid_ids_ = CalculateAllValidIds(valid_id_ranges, available_ids);
    }

    Ids CalculateAvailableValidIds(const ValidIdRanges& valid_id_ranges, const Ids& available_ids)
    {
        Ids available_valid_ids{};

        for (const auto& available_id : available_ids)
        {
            for (const auto& valid_id_range : valid_id_ranges)
            {
                if (available_id >= valid_id_range.first && available_id <= valid_id_range.second)
                {
                    available_valid_ids.emplace(available_id);
                    break;
                }
            }
        }

        return available_valid_ids;
    }

    std::size_t CalculateAllValidIds(ValidIdRanges valid_id_ranges, const Ids& available_ids)
    {
        std::ranges::sort(valid_id_ranges, [](const auto& a, const auto& b) { return a.first < b.first; });

        ValidIdRanges merged_unique_ranges;
        for (const auto& range : valid_id_ranges)
        {
            if (merged_unique_ranges.empty() || merged_unique_ranges.back().second < range.first - 1)
            {
                merged_unique_ranges.push_back(range);
            }
            else
            {
                merged_unique_ranges.back().second = std::max(merged_unique_ranges.back().second, range.second);
            }
        }

        std::size_t counter{0};
        for (const auto& valid_id_range : merged_unique_ranges)
        {
            counter += valid_id_range.second - valid_id_range.first + 1;
        }

        return counter;
    }

    Ids GetAvailableValidIds() { return available_valid_ids_; }
    std::size_t GetAllValidIds() { return number_of_all_valid_ids_; }

  private:
    Ids available_valid_ids_{};
    std::size_t number_of_all_valid_ids_{};
};

inline void Run()
{
    FileParser parser{ReadFile("2025/day5/example.txt")};
    CalculateValidIds calculate_valid_ids{parser.ViewValidIdRanges(), parser.ViewAvailableIds()};
    std::cout << "Part 1 - Number of available valid ids is " << calculate_valid_ids.GetAvailableValidIds().size()
              << std::endl;
    std::cout << "Part 2 - Number of all valid ids is " << calculate_valid_ids.GetAllValidIds() << std::endl;
}