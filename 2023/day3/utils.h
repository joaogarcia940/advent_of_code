#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <regex>
#include <unordered_map>
#include <cassert>
#include <limits>
#include <numeric>
#include <algorithm>
#include <map>
#include "utils/utils.h"

class Puzzle
{
public:
    uint64_t GetSum()
    {
        return std::accumulate(numbers_.begin(), numbers_.end(), 0,
                               [](const std::size_t previous, const auto &element)
                               { return previous + element.second; });
    }

    uint64_t GetSumOfGearRatios()
    {
        return std::accumulate(gears_.begin(), gears_.end() - 1, 0, [](const std::size_t previous, const auto &ratios)
                               { return previous + std::accumulate(ratios.begin(), ratios.end(), 1.0, std::multiplies<uint64_t>()); });
    }

    void FillNumbersVector(const std::string &line, size_t special_char_pos, size_t line_number)
    {
        const auto number_around_char_in_line =
            FillNumbersAroundCharInLine(line, special_char_pos, line_number);

        const auto &r_number = number_around_char_in_line.r_number_with_pos.number;
        const auto r_number_x = number_around_char_in_line.r_number_with_pos.x;
        const auto &number = number_around_char_in_line.number_with_pos.number;
        const auto number_x = number_around_char_in_line.number_with_pos.x;
        const auto y = number_around_char_in_line.number_with_pos.y;

        if (std::isdigit(line[special_char_pos]))
        {
            if (r_number.empty())
            {
                numbers_[std::to_string(number_x - 1) + ',' + std::to_string(y)] = std::stoi(line[special_char_pos] + number);
            }
            else if (number.empty())
            {
                numbers_[std::to_string(r_number_x) + ',' + std::to_string(y)] = std::stoi(r_number + line[special_char_pos]);
            }
            else if (r_number.empty() && number.empty())
            {
                numbers_[std::to_string(special_char_pos) + ',' + std::to_string(y)] = line[special_char_pos] - '0';
            }
            else
            {
                numbers_[std::to_string(r_number_x) + ',' + std::to_string(y)] = std::stoi(r_number + line[special_char_pos] + number);
            }
        }
        else
        {
            if (not r_number.empty())
            {
                numbers_[std::to_string(r_number_x) + ',' + std::to_string(y)] = std::stoi(r_number);
            }
            if (not number.empty())
            {
                numbers_[std::to_string(number_x - 1) + ',' + std::to_string(y)] = std::stoi(number);
            }
        }
    }

    void FillPotentialGearsInRow(size_t x_star_pos, size_t y, const std::string &line)
    {
        auto uncertainty = GetMaxNumDigitsInRow(y);
        auto min_pos = std::max(x_star_pos - uncertainty, 0LU);
        auto max_pos = x_star_pos + 1;
        for (auto i = min_pos; i < max_pos; i++)
        {
            auto match = numbers_.find(GetKey(i, y));
            if (match != numbers_.end())
            {
                if (i + GetNumDigits((*match).second) > (x_star_pos - 1))
                {
                    gears_.empty() ? gears_.push_back(std::vector<uint64_t>{(*match).second})
                                   : gears_.back().push_back((*match).second);
                }
            }
        }
    }

    void TrimAndChangeGear()
    {
        if ((!gears_.empty()) && (gears_.back().size() != 2))
        {
            gears_.pop_back();
        }
        gears_.push_back(std::vector<uint64_t>{});
    }

private:
    struct NumbersAroundCharInLine
    {
        struct NumberWithPos
        {
            std::string number;
            size_t x;
            size_t y;
        };
        NumberWithPos r_number_with_pos;
        NumberWithPos number_with_pos;
    };

    NumbersAroundCharInLine FillNumbersAroundCharInLine(const std::string &line, size_t special_char_pos, size_t line_number)
    {
        std::string r_number{};
        std::string number{};
        size_t r_pos = 0;
        for (auto r_iter = std::make_reverse_iterator(line.begin() + special_char_pos); (r_iter != line.rend()); ++r_iter) // make all this safe
        {
            if (!std::isdigit(*r_iter))
            {
                break;
            }
            else
            {
                r_number.push_back(*r_iter);
            }
            r_pos++;
        }
        std::reverse(r_number.begin(), r_number.end());

        auto iter = line.begin() + special_char_pos + 1;
        for (; iter != line.end(); ++iter)
        {
            if (!std::isdigit(*iter))
            {
                break;
            }
            else
            {
                number.push_back(*iter);
            }
        }
        return NumbersAroundCharInLine{
            {r_number, special_char_pos - r_pos, line_number},
            {number, special_char_pos + 1, line_number}};
    }

    std::pair<size_t, size_t> GetDecodedCoordinates(std::string key)
    {
        return std::make_pair(std::stoi(key), std::stoi(key.substr(key.find(',') + 1, key.length())));
    }

    size_t GetMaxNumDigitsInRow(size_t y)
    {
        auto numbers_in_row = GetNumbersInRow(y);
        if (numbers_in_row.empty())
        {
            return 0;
        }
        return GetNumDigits(*std::max_element(numbers_in_row.begin(), numbers_in_row.end()));
    }

    size_t GetNumDigits(size_t num)
    {
        size_t count = 0;
        while (num)
        {
            num /= 10;
            count++;
        }
        return count;
    }

    std::string GetKey(size_t x, size_t y)
    {
        return std::to_string(x) + ',' + std::to_string(y);
    }

    std::vector<uint64_t> GetNumbersInRow(size_t y)
    {
        std::vector<uint64_t> numbers_in_row;

        for (const auto num : numbers_)
        {
            if (y == GetDecodedCoordinates(num.first).second)
            {
                numbers_in_row.push_back(num.second);
            }
        }
        return numbers_in_row;
    }

private:
    std::unordered_map<std::string, uint64_t> numbers_;
    std::vector<std::vector<uint64_t>> gears_;
};

class FileParserDay3 : public FileParser<FileParserDay3> // crtp / decorator
{
public:
    FileParserDay3 &Filter() override
    {
        assert(text_.begin() != text_.end());
        for (size_t i = 2; i < text_.size(); ++i)
        {
            const auto &above = text_[i - 2];
            const auto &middle = text_[i - 1];
            const auto &below = text_[i];

            for (size_t pos = 0; pos < middle.size(); ++pos)
            {
                if (!std::isdigit(middle[pos]) && !(middle[pos] == '.'))
                {
                    puzzle_.FillNumbersVector(above, pos, i - 2);
                    puzzle_.FillNumbersVector(middle, pos, i - 1);
                    puzzle_.FillNumbersVector(below, pos, i);

                    if (middle[pos] == '*')
                    {
                        puzzle_.FillPotentialGearsInRow(pos, i - 2, above);
                        puzzle_.FillPotentialGearsInRow(pos, i - 1, middle);
                        puzzle_.FillPotentialGearsInRow(pos, i, below);
                        puzzle_.TrimAndChangeGear();
                    }
                }
            }
        }

        return *this;
    }

    Puzzle GetPuzzle()
    {
        return puzzle_;
    }

private:
    Puzzle puzzle_;
};
