#include "utils.h"

int main()
{
    const std::string path_in = "/home/joao/workspace/advent/day1/input.txt";
    const std::string path_out = "/home/joao/workspace/advent/day1/part1.txt";
    FileParserDay1 parse_first_numbers;
    const auto first_numbers = parse_first_numbers.WithPattern(std::regex(R"(\d)")).WithTextFromFile(path_in).Filter().Get();
    FileParserDay1 parse_last_numbers;
    const auto last_numbers = parse_last_numbers.WithPattern(std::regex(R"(\d)")).WithTextFromFile(path_in).WithReversedText().Filter().Get(); //crtp
    const auto merged_numbers = MergeTextNumbers(first_numbers, last_numbers);
    std::ofstream(path_out) << SumNumbersInText(merged_numbers) << std::endl;

    return 0;
}
