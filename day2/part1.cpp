#include "utils.h"

int main()
{
    const std::string path_in = "/home/joao/workspace/advent/day2/input.txt";
    const std::string path_out = "/home/joao/workspace/advent/day2/part1.txt";
    FileParserDay2 parser;
    const auto id_sum = parser.WithTextFromFile(path_in).Filter().GetGeneralGameData().GetIdSum();
    std::ofstream(path_out) << id_sum << std::endl;

    return 0;
}
