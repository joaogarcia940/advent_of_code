//
// Created by CTW00728 on 12/3/2024.
//

#ifndef ADVENT_DAY3_H
#define ADVENT_DAY3_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <string_view>
#include <optional>
#include <sstream>
#include <algorithm>
#include <numeric>
#include "../utils/file_reader.h"
#include "../utils/numbers_in_string.h"
#include "../utils/print_vector.h"
#include <regex>

namespace day3 {
    enum class Part {
        One,
        Two
    };

    using Multiplications = std::vector<std::pair<long, long>>;

    constexpr std::string_view example = "day3/example.txt";
    constexpr std::string_view part1 = "day3/part1.txt";

    std::vector<long> GetNumbersInString(const std::string &str) {
        auto regex = std::regex(R"(\d+)");
        auto it = std::sregex_iterator{str.cbegin(), str.cend(), regex};
        auto end = std::sregex_iterator();
        std::vector<long> numbers;
        for (; it != end; ++it) {
            numbers.push_back(stol((*it).str()));
        }
        return numbers;
    }

    class FileParser {
    public:
        explicit FileParser(Text &&text, Part part) : multiplications_{} {
            switch (part) {
                case Part::One:
                    Part1(text);
                    break;
                case Part::Two:
                    Part2(text);
                    break;
                default:
                    std::cout << "Error choosing Part" << std::endl;
            }

        }

        Multiplications GetMultiplications() {
            return multiplications_;
        }

    private:
        void Part1(const Text &text) {
            for (auto &line: text) {
                auto mul_regex = std::regex(R"(mul\(\d+,\d+\))");
                auto it_mul = std::sregex_iterator{line.cbegin(), line.cend(), mul_regex};

                auto end = std::sregex_iterator();

                if (it_mul != end) {
                    std::vector<std::string> matches;
                    for (; it_mul != end; ++it_mul) {
                        const auto numbers = GetNumbersInString((*it_mul).str());
                        multiplications_.emplace_back(numbers.front(), numbers.back()); // Append matches
                    }
                } else {
                    std::cout << "Mul pattern couldn't find anything in line " << line << std::endl;
                }
            }
        }

        void Part2(const Text &text) {
            bool do_ = true;
            bool dont = false;
            bool mul = false;
            for (auto &line: text) {
                auto regex = std::regex(R"((do\(\)|don't\(\)|mul\(\d+,\d+\)))");
                auto it = std::sregex_iterator{line.cbegin(), line.cend(), regex};

                auto end = std::sregex_iterator();

                if (it != end) {
                    std::vector<std::string> matches;
                    std::string match;
                    for (; it != end; ++it) {
                        match = it->str();
                        mul = match.substr(0, 3) == "mul";
                        if (mul && do_ && !dont) {
                            const auto numbers = GetNumbersInString(match);
                            if (numbers.size() != 2) {
                                std::cerr << "problem decoding the numbers" << std::endl;
                            }
                            multiplications_.emplace_back(numbers.front(), numbers.back());
                        }

                        if (dont) {
                            if (match == "do()") {
                                do_ = true;
                                dont = false;
                            }
                        }
                        if (do_) {
                            if (match == "don't()") {
                                dont = true;
                                do_ = false;
                            }
                        }
                    }
                } else {
                    std::cout << "Pattern couldn't find anything in line " << line << std::endl;
                }
            }
        }

        Multiplications multiplications_;
    };

    class Result {
    public:
        explicit Result(Multiplications &&multiplications) : result{0} {
            for (auto &multiplication: multiplications) {
                result += multiplication.first * multiplication.second;
            }
        }

        auto GetResult() {
            return result;
        }

    private:
        long result;
    };

    void Run() {
        FileReader file_reader{part1};
        FileParser file_parser_part_1{file_reader.GetText(), Part::One};
        Result part_1_result{file_parser_part_1.GetMultiplications()};
        std::cout << "Part 1: Sum of multiplications is " << part_1_result.GetResult() << std::endl;
        FileParser file_parser_part_2{file_reader.GetText(), Part::Two};
        Result part_2_result{file_parser_part_2.GetMultiplications()};
        std::cout << "Part 2: Sum of multiplications is " << part_2_result.GetResult() << std::endl;
    }
}

#endif //ADVENT_DAY3_H
