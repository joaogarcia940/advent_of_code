#include "../common/read_file.h"
#include <algorithm>
#include <vector>

using Joltage = std::size_t;
using Bank = std::vector<Joltage>;
using Battery = std::vector<Bank>;
using NumDigits = std::size_t;

constexpr NumDigits kPart1{2};
constexpr NumDigits kPart2{12};

class FileParser
{
  public:
    explicit FileParser(const Text& text) : banks_{}
    {
        for (const auto& line : text)
        {
            Bank bank{};
            for (const auto ch : line)
            {
                bank.push_back(ch - '0');
            }
            banks_.push_back(bank);
        }
    }

    const Battery& ViewBattery() const { return banks_; }

  private:
    Battery banks_{};
};

class HighestJoltage
{
  public:
    explicit HighestJoltage(const Battery& banks, const NumDigits num_digits)
    {
        CalculateHighestJoltage(banks, num_digits);
    }

    Joltage GetHighestJoltageCounter() const { return highest_joltage_counter_; }

  private:
    void CalculateHighestJoltage(const Battery& banks, const NumDigits num_digits)
    {
        for (auto bank : banks)
        {
            std::vector<std::vector<Joltage>::const_iterator> digit_its(num_digits, bank.cbegin());
            digit_its[0] = std::max_element(bank.cbegin(), bank.cend() - num_digits + 1);
            for (std::size_t i = 1; i < num_digits; ++i)
            {
                const auto begin = digit_its[i - 1] + 1;
                const auto end = bank.cend() - num_digits + 1 + i;
                digit_its[i] = std::max_element(begin, end);
            }
            std::string highest_joltage_str{};
            for (const auto it : digit_its)
            {
                highest_joltage_str += std::to_string(*it);
            }
            // std::print("Highest joltage for bank is {}\n", highest_joltage_str);
            highest_joltage_counter_ += std::stoul(highest_joltage_str);
        }
    }

    Joltage highest_joltage_counter_{0};
};

inline void Run()
{
    const FileParser parser{ReadFile("2025/day3/real.txt")};
    const auto highest_joltage = HighestJoltage{parser.ViewBattery(), kPart1};
    std::print("Part 1 - Highest joltage counter is {}\n", highest_joltage.GetHighestJoltageCounter());  // 17452
    const auto highest_joltage_part2 = HighestJoltage{parser.ViewBattery(), kPart2};
    std::print("Part 2 - Highest joltage counter is {}\n", highest_joltage_part2.GetHighestJoltageCounter());  // 173300819005913
}