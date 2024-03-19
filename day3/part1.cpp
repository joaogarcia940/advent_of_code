#include "utils.h"

int main()
{
    const std::string path_in = "/home/joao/workspace/advent/day3/input.txt";
    const std::string path_out = "/home/joao/workspace/advent/day3/part1.txt";
    FileParserDay3 parser;
    const auto id_sum = parser.WithTextFromFile(path_in).Filter().GetPuzzle().GetSum();
    std::ofstream(path_out) << id_sum << std::endl;

    return 0;
}
