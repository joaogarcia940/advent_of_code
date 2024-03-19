#include "utils.h"

int main()
{
    const std::string path_in = "/home/joao/workspace/advent/day2/input.txt";
    const std::string path_out = "/home/joao/workspace/advent/day2/part2.txt";
    FileParserDay2 parser;
    const auto power_sum = parser.WithTextFromFile(path_in).Filter().GetGeneralGameData().GetPowerSum();
    std::ofstream(path_out) << power_sum << std::endl;

    return 0;
}
