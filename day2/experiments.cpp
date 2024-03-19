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

int main()
{
    std::string line = "Game 1: 9 red, 5 blue, 6 green; 6 red, 13 blue; 2 blue, 7 green, 5 red";

    std::istringstream input_string_stream(line);
    char comma = ',', semicolon = ';';
    std::unordered_map<Color, int> color_count{};
    int id;
    input_string_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    input_string_stream >> id;
    input_string_stream.ignore();

    std::cout << input_string_stream.str() << std::endl;
    while (not input_string_stream.eof())
    {
        int count;
        std::string color;
        // Reading counts and colors in a set

        input_string_stream >> count >> color;
        color.erase(remove_if(color.begin(), color.end(), [](char c)
                              { return ((c == ',') || (c == ';')); }),
                    color.end());
        if (color_count.find(ToEnum(color)) == color_count.end())
        {
            printf("color: %s, count: %d", color.c_str(), count);
            color_count.emplace(std::make_pair(ToEnum(color), count));
        }
        else
        {
            color_count[ToEnum(color)] = std::max(count, color_count[ToEnum(color)]);
        }
        input_string_stream.ignore(); // skip the comma
    }
}
