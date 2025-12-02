#include "../utils/file_reader.h"
#include "../utils/numbers_in_string.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace day7
{

using Number = long;
using Numbers = std::vector<long>;

struct Equation
{
    Number result;
    Numbers operands;
};

using Equations = std::vector<Equation>;

class FileParser
{
  public:
    explicit FileParser(Text&& text) : equations_{}
    {
        for (auto&& line : text)
        {
            const auto numbers = GetNumbersInString(std::move(line));
            equations_.push_back({numbers.front(), Numbers(numbers.cbegin() + 1, numbers.cend())});
        }
    }

    Equations GetEquations() const { return equations_; }

  private:
    Equations equations_{};
};

class Solver
{
  public:
    explicit Solver(const Equations& equations, std::vector<std::string_view>&& operators)
    {
        for (const auto& equation : equations)
        {
            // find all combinations of + and * that yield the result from the operands
            const auto expressions = GetAllPossibleExpressions(equation.operands, operators);
            for (const auto& expression : expressions)
            {
                const auto expression_result = EvaluateExpression(expression);
                if (expression_result == equation.result)
                {
                    counter_ += equation.result;
                    ++different_possibilities_;
                    break;
                }
            }
        }
    }

    Number GetCounter() const { return counter_; }
    Number GetDifferentPossibilities() const { return different_possibilities_; }

  private:
    std::vector<std::string> GetAllPossibleExpressions(const Numbers& operands,
                                                       const std::vector<std::string_view>& operators)
    {
        const auto k_base = operators.size();
        const auto slots = operands.size() - 1;
        const auto num_combinations =
            static_cast<std::size_t>(std::pow(static_cast<double>(k_base), static_cast<double>(slots)));

        std::vector<std::string> expressions;
        for (std::size_t combination = 0; combination < num_combinations; ++combination)
        {
            std::string expression;
            expression += std::to_string(operands.front());
            auto factored = combination;
            for (std::size_t j = 0; j < slots; ++j)
            {
                const auto operator_index = factored % k_base;
                expression += " ";
                expression += operators[operator_index];
                expression += " ";
                expression += std::to_string(operands[j + 1]);
                factored /= k_base;
            }
            expressions.push_back(expression);
        }
        return expressions;
    }

    Number EvaluateExpression(std::string expression)
    {
        std::string operation{""};

        auto stream = std::istringstream(expression);
        Number result{0};
        Number current_number{0};
        operation = "+";
        while (stream >> current_number)
        {
            if (operation == "+")
            {
                result += current_number;
            }
            else if (operation == "*")
            {
                result *= current_number;
            }
            else if (operation == "||")
            {
                result = std::stol(std::to_string(result) + std::to_string(current_number));
            }
            else
            {
                // throw std::runtime_error("Invalid operation");
            }
            stream >> operation;
        }

        return result;
    }

    std::string Concatenate(std::string expression)
    {
        std::istringstream stream(expression);
        std::string result;
        Number current_number{0};
        std::string operation{""};

        stream >> current_number;
        result += std::to_string(current_number);

        return result;
    }

    Number counter_{0};
    Number different_possibilities_{0};
};

}  // namespace day7

int main()
{
    FileReader reader{"2024/day7/real.txt"};
    day7::FileParser parser{reader.GetText()};
    const auto equations = parser.GetEquations();
    day7::Solver part_1{equations, {"+", "*"}};
    std::cout << "Part1" << std::endl;
    std::cout << "Counter: " << part_1.GetCounter() << std::endl;
    std::cout << "Different possibilities: " << part_1.GetDifferentPossibilities() << std::endl;
    day7::Solver part_2{equations, {"+", "*", "||"}};
    std::cout << "Part2" << std::endl;
    std::cout << "Counter: " << part_2.GetCounter() << std::endl;
    std::cout << "Different possibilities: " << part_2.GetDifferentPossibilities() << std::endl;
    return 0;
}
