#include "utils.h"

int main()
{
    const std::string path_in = "/home/joao/workspace/advent/day4/input.txt";
    const std::string path_out = "/home/joao/workspace/advent/day4/part1.txt";
    FileParserDay4 parser;
    const auto id_sum = parser.WithTextFromFile(path_in).Filter().GetGame().GetPoints();
    std::ofstream(path_out) << id_sum << std::endl;

    return 0;
}
