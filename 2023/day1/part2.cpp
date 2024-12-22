#include "utils.h"

const static auto string_to_num = std::unordered_map<std::string, std::string>{
    {"zero", "0"},
    {"one", "1"},
    {"two", "2"},
    {"three", "3"},
    {"four", "4"},
    {"five", "5"},
    {"six", "6"},
    {"seven", "7"},
    {"eight", "8"},
    {"nine", "9"},
};

const static auto string_to_num_reversed = std::unordered_map<std::string, std::string>{
    {"orez", "0"},
    {"eno", "1"},
    {"owt", "2"},
    {"eerht", "3"},
    {"ruof", "4"},
    {"evif", "5"},
    {"xis", "6"},
    {"neves", "7"},
    {"thgie", "8"},
    {"enin", "9"},
};

int main()
{
    const std::string path_in = "/home/joao/workspace/advent/day1/input.txt";
    const std::string path_out = "/home/joao/workspace/advent/day1/part2.txt";
    FileParserDay1 parse_first_numbers;
    const auto first_numbers =
        parse_first_numbers.WithPattern(
                               std::regex(R"((?:zero|one|two|three|four|five|six|seven|eight|nine|(\d)))"))
            .WithTextFromFile(path_in)
            .Filter()
            .WithExtensiveNumberSubstitution(string_to_num)
            .Get();
    FileParserDay1 parse_last_numbers;
    const auto last_numbers =
        parse_last_numbers.WithPattern(
                              std::regex(R"((?:orez|eno|owt|eerht|ruof|evif|xis|neves|thgie|enin|(\d)))"))
            .WithTextFromFile(path_in)
            .WithReversedText()
            .Filter()
            .WithExtensiveNumberSubstitution(string_to_num_reversed)
            .Get();
    const auto merged_numbers = MergeTextNumbers(first_numbers, last_numbers);
    std::ofstream(path_out) << SumNumbersInText(merged_numbers) << std::endl;

    return 0;
}
