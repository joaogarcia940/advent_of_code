#pragma once

#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <regex>
#include <unordered_map>
#include <cassert>
#include <limits>
#include "utils/utils.h"

using Count = int;

enum class Color : u_int
{
    Red,
    Green,
    Blue,
    Invalid,
};

Color ToEnum(std::string color)
{
    if (color == "red")
        return Color::Red;
    if (color == "green")
        return Color::Green;
    if (color == "blue")
        return Color::Blue;
    return Color::Invalid;
}

class GeneralGame
{
public:
    void Update(const std::unordered_map<Color, Count> &color_count, int id)
    {
        if (IsGameValid(color_count))
        {
            id_sum_ += id;
        }
        power_sum_ += color_count.at(Color::Red) * color_count.at(Color::Green) * color_count.at(Color::Blue);
    }

    int GetIdSum()
    {
        return id_sum_;
    }

    int GetPowerSum()
    {
        return power_sum_;
    }

private:
    bool IsColorValid(const std::unordered_map<Color, Count> &color_count, const Color color)
    {
        if (color_count.find(color) != color_count.end())
        {
            return color_count.at(color) <= num_color_cubes.at(color);
        }
        return false;
    }
    bool IsGameValid(const std::unordered_map<Color, Count> &color_count)
    {
        return IsColorValid(color_count, Color::Red) &&
               IsColorValid(color_count, Color::Green) &&
               IsColorValid(color_count, Color::Blue);
    }

    const std::unordered_map<Color, Count> num_color_cubes{{Color::Red, 12}, {Color::Green, 13}, {Color::Blue, 14}};
    int id_sum_ = 0;
    int power_sum_ = 0;
};

class FileParserDay2 : public FileParser<FileParserDay2> // crtp / decorator
{
public:
    FileParserDay2 &Filter() override
    {
        assert(text_.begin() != text_.end());
        std::for_each(text_.begin(), text_.end(), [this](std::string &line)
                      {
            std::istringstream input_string_stream(line);
            std::unordered_map<Color, int> color_count{{Color::Red, 0}, {Color::Green, 0}, {Color::Blue, 0}};
            int id;

            input_string_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' '); // ignore until :
            input_string_stream >> id;
            input_string_stream.ignore();
            while (not input_string_stream.eof())
            {
                int count;
                std::string color;

                input_string_stream >> count >> color;
                color.erase(remove_if(color.begin(), color.end(), [](char c)
                                      { return ((c == ',') || (c == ';')); }),
                            color.end());
                if (color_count.find(ToEnum(color)) == color_count.end())
                {
                    color_count.emplace(std::make_pair(ToEnum(color), count));
                }
                else
                {
                    color_count[ToEnum(color)] = std::max(count, color_count[ToEnum(color)]);
                }
                input_string_stream.ignore(); // skip the comma
            }
            general_game_.Update(color_count, id); });

        return *this;
    }

    GeneralGame GetGeneralGameData()
    {
        return general_game_;
    }

private:
    GeneralGame general_game_;
};