#include "../common/read_file.h"
#include <ranges>
#include <string>
#include <vector>

using Number = std::size_t;
using Counter = std::size_t;

struct Range
{
    Number start;
    Number end;
};

using Ranges = std::vector<Range>;

class FileParser
{
  public:
    explicit FileParser(const Text& text)
    {
        if (text.size() != 1)
        {
            throw std::runtime_error("Expected exactly one line of input");
        }
        const auto& line = text.front();
        const auto parsed =
            line | std::views::split(',') |
            std::views::transform([](auto&& range) { return std::string(range.begin(), range.end()); }) |
            std::ranges::to<std::vector>();
        for (const auto& item : parsed)
        {
            ranges_.push_back(ExtractRange(item));
        }
    }

    const Ranges& ViewRanges() const { return ranges_; }

  private:
    Range ExtractRange(const std::string& item)
    {
        auto parts = item | std::views::split('-') |
                     std::views::transform([](auto&& number) { return std::string(number.begin(), number.end()); }) |
                     std::ranges::to<std::vector>();
        if (parts.size() != 2)
        {
            throw std::runtime_error("Invalid range: " + item);
        }
        return Range{std::stoul(parts.front()), std::stoul(parts.back())};
    }

    Ranges ranges_{};
};

class InvalidIdsDetector
{
  public:
    explicit InvalidIdsDetector(const Ranges& ranges) : ranges_{ranges} { FindRepeatedNumbersTwiceInRanges(ranges_); }

    Counter GetCountInvalidIdsPart1() const { return count_invalid_ids_part1_; }

    Counter GetSumInvalidIdsPart1() const { return sum_invalid_ids_part1_; }

    Counter GetCountInvalidIdsPart2() const { return count_invalid_ids_part2_; }

    Counter GetSumInvalidIdsPart2() const { return sum_invalid_ids_part2_; }

  private:
    void FindRepeatedNumbersTwiceInRanges(const Ranges& ranges)
    {
        for (const auto& range : ranges)
        {
            for (Number number = range.start; number <= range.end; ++number)
            {
                if (IsRepeatedNumberTwice(number))
                {
                    ++count_invalid_ids_part1_;
                    sum_invalid_ids_part1_ += number;
                }
                if (IsRepeatedNumberAtLeastTwice(number))
                {
                    // std::print("Invalid ID Part 2: {}\n", number);
                    ++count_invalid_ids_part2_;
                    sum_invalid_ids_part2_ += number;
                }
            }
        }
    }

    bool IsRepeatedNumberTwice(const Number number)
    {
        const auto number_of_digits = std::to_string(number).size();
        const auto number_str = std::to_string(number);
        const auto first_half = number_str.substr(0, number_of_digits / 2);
        const auto second_half = number_str.substr(number_of_digits / 2, number_of_digits);
        return first_half == second_half;
    }

    bool IsRepeatedNumberAtLeastTwice(const Number number)
    {
        const auto number_of_digits = std::to_string(number).size();
        const auto number_str = std::to_string(number);
        for (std::size_t divider = 2; divider <= number_of_digits && number_of_digits > 1; ++divider)
        {
            // std::print("Number is {}; Number of digits is {}; Trying divider {}\n", number, number_of_digits, divider);
            if (number_of_digits % divider != 0)
            {
                continue;
            }

            // split string into parts of size divider
            const auto part_size = number_of_digits / divider;
            const auto start = number_str.substr(0, part_size);
            if (IsRepeatedNumberAtLeastTwice(number_str, part_size, number_of_digits, start))
            {
                return true;
            }
        }
        const auto first_half = number_str.substr(0, number_of_digits / 2);
        const auto second_half = number_str.substr(number_of_digits / 2, number_of_digits);
        return first_half == second_half;
    }

    bool IsRepeatedNumberAtLeastTwice(const auto& number_str,
                                      const auto part_size,
                                      const auto number_of_digits,
                                      const auto& start)
    {
        for (std::size_t compare_start = part_size; compare_start < number_of_digits; compare_start += part_size)
        {
            // std::print("\tComparing {} with {}\n", start, number_str.substr(compare_start, part_size));
            const auto compare_part = number_str.substr(compare_start, part_size);
            if (start != compare_part)
            {
                return false;
            }
        }
        return true;
    }

    Ranges ranges_{};
    Counter count_invalid_ids_part1_{0};
    Counter sum_invalid_ids_part1_{0};
    Counter count_invalid_ids_part2_{0};
    Counter sum_invalid_ids_part2_{0};
};

inline void Run()
{
    const FileParser parser{ReadFile("2025/day2/real.txt")};
    const auto detector = InvalidIdsDetector{parser.ViewRanges()};
    std::print("Part 1 - Invalid  {}, Sum {}\n", detector.GetCountInvalidIdsPart1(), detector.GetSumInvalidIdsPart1());
    std::print("Part 2 - Invalid  {}, Sum {}\n", detector.GetCountInvalidIdsPart2(), detector.GetSumInvalidIdsPart2());
}