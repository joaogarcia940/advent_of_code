#include "../common/read_file.h"
#include <ranges>
#include <algorithm>

using Number = std::size_t;
using Numbers = std::vector<Number>;
using Equation = std::vector<Number>;
using Equations = std::vector<Equation>;
using Operation = char;
using StringVector = std::vector<std::string>;
using ParsedText = std::vector<StringVector>;

inline Number GetOperationResult(const Number lhs, const Operation operation, const Number rhs)
{
    switch (operation)
    {
        case '+':
            return lhs + rhs;
        case '*':
            return lhs * rhs;
    }
    throw std::runtime_error("Invalid operation: " + std::string{operation});
}

Number GetNeutralElementForOperation(const Operation operation)
{
    switch (operation)
    {
        case '+':
            return 0;
        case '*':
            return 1;
    }
    throw std::runtime_error("Invalid operation: " + std::string{operation});
}

class Day6Part1
{
  public:
    Day6Part1(const Text& text) : count_{}
    {
        ParsedText parsed_text;
        for (const auto& line : text)
        {
            StringVector parts =
                line | std::views::split(' ') | std::views::filter([](const auto& part) { return !part.empty(); }) |
                std::views::transform([](const auto& part) { return std::ranges::to<std::string>(part); }) |
                std::ranges::to<std::vector>();

            parsed_text.push_back(std::move(parts));
        }

        const auto number_of_operands = parsed_text.size() - 1;
        const auto number_of_equations = parsed_text.front().size();
        equations_.reserve(number_of_equations);
        for (Number col{0}; col < number_of_equations; ++col)
        {
            const Operation operation{parsed_text[parsed_text.size() - 1][col].front()};
            auto solution = GetNeutralElementForOperation(operation);
            for (Number row{0}; row < number_of_operands; ++row)
            {
                solution = GetOperationResult(solution, operation, std::stol(parsed_text[row][col]));
            }
            count_ += solution;
        }
    }

    Number GetSolution() const { return count_; }

  private:
    Equations equations_{};
    Number count_{};
};

class Day6Part2
{
  public:
    Day6Part2(const Text& text) : grand_total_{}
    {
        const auto num_rows = text.size();
        const auto num_columns = text.front().size();
        for (Number col{0}; col < num_columns; ++col)
        {
            std::string operation_col_str{GetColumnString(text, col, num_rows)};
            if (!IsOperation(operation_col_str.back()))
            {
                throw "Logic error in column " + std::to_string(col);
            }
            const auto operation = operation_col_str.back();
            Number solution{GetNeutralElementForOperation(operation)};
            for (std::string col_str{operation_col_str.cbegin(), operation_col_str.cend() - 1};
                 !IsAllWhitespace(col_str) && col < num_columns;
                 col_str = GetColumnString(text, ++col, num_rows))
            {
                solution = GetOperationResult(solution, operation, std::stol(col_str));
            }
            grand_total_ += solution;
        }
    }

    Number GetSolution() { return grand_total_; }

  private:
    bool IsOperation(const char ch) { return ch == '+' || ch == '*'; }
    bool IsAllWhitespace(const std::string& str) { return std::ranges::all_of(str, ::isspace); }
    std::string GetColumnString(const Text& text, const Number col, const Number num_rows)
    {
        std::string col_str{};
        for (Number row{0}; row < num_rows; ++row)
        {
            col_str += text[row][col];
        }
        return col_str;
    }

    Number grand_total_{};
};

inline void Run()
{
    const auto text = ReadFile("2025/day6/real.txt");
    Day6Part1 part_1{text};
    std::print("Part 1 - {}\n", part_1.GetSolution());
    Day6Part2 part_2{text};
    std::print("Part 2 - {}\n", part_2.GetSolution());
}
