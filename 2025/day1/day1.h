#include <print>
#include <ranges>
#include <fstream>
#include <string>
#include <vector>

using Text = std::vector<std::string>;
using Counter = std::size_t;

inline Text ReadFile(const std::string& path)
{
    std::ifstream file{path};
    if (!file.good() && !file.eof())
    {
        throw std::runtime_error("Error reading file: " + path);
    }

    const auto begin = std::istreambuf_iterator<char>(file);
    const auto end = std::istreambuf_iterator<char>();

    std::string all_content{begin, end};

    return all_content | std::views::split('\n') |
           std::views::transform([](auto&& str_range) { return std::string(str_range.begin(), str_range.end()); }) |
           std::ranges::to<std::vector>();
}

enum class Direction
{
    Left,
    Right,
};

using DialValue = long;

struct Command
{
    Direction direction;
    DialValue distance;
};

using Commands = std::vector<Command>;

class FileParser
{
  public:
    explicit FileParser(const Text& text) : commands_{}
    {
        for (const auto& line : text)
        {
            commands_.push_back(ExtractCommand(line));
        }
    }

    const Commands& ViewCommands() const { return commands_; }

  private:
    Command ExtractCommand(const std::string& line)
    {
        Command result{};

        if (line[0] == 'L')
        {
            result.direction = Direction::Left;
        }
        else if (line[0] == 'R')
        {
            result.direction = Direction::Right;
        }
        else
        {
            throw std::runtime_error("Invalid direction");
        }
        result.distance = {std::stol(line.substr(1))};
        return result;
    }

    Commands commands_{};
};

constexpr DialValue kInitialValue{50};
class DialRotator
{
  public:
    DialRotator(const Commands& commands) { RunCommands(commands); }

    Counter GetCountFinishedIn0() const { return count_finished_in_0_; }
    Counter GetCountPassed0() const { return count_passed_0_; }

  private:
    void RunCommands(const Commands& commands)
    {
        DialValue current_value{kInitialValue};
        for (const auto& command : commands)
        {
            current_value = RotateDial(command, current_value);
            if (current_value == 0)
            {
                ++count_finished_in_0_;
                was_previous_zero_ = true;
            }
            else
            {
                was_previous_zero_ = false;
            }
        }
    }

    DialValue RotateDial(const Command& command, DialValue current_value)
    {
        const auto previous_value = current_value;
        switch (command.direction)
        {
            case Direction::Left:
            {
                current_value -= command.distance;
                break;
            }
            case Direction::Right:
            {
                current_value += command.distance;
                break;
            }
        }

        return NormalizeDialValueAndCountPassed0s(previous_value, current_value);
    }

    DialValue NormalizeDialValueAndCountPassed0s(const DialValue previous_value, DialValue value)
    {
        if (previous_value * value < 0)  // crossed zero
        {
            ++count_passed_0_;
        }
        if (value == 0)
        {
            ++count_passed_0_;
            return value;
        }
        while (value <= -100)
        {
            value += 100;
            ++count_passed_0_;
        }
        while (value >= 100)
        {
            value -= 100;
            ++count_passed_0_;
        }
        if (value < 0)  // Normalize negative values to positive
        {
            value += 100;
        }
        if (value < 0)
        {
            throw std::runtime_error("Normalization failed, value is still negative");
        }

        return value;
    }

    Counter count_finished_in_0_{0};
    Counter count_passed_0_{0};
    bool was_previous_zero_{false};
};

inline void Run()
{
    FileParser parser{ReadFile("day1/real.txt")};
    DialRotator rotator{parser.ViewCommands()};
    std::print("Part 1 - {}\n", rotator.GetCountFinishedIn0());
    std::print("Part 2 - {}\n", rotator.GetCountPassed0());
}