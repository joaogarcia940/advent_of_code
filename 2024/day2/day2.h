//
// Created by CTW00728 on 12/3/2024.
//

#ifndef ADVENT_DAY2_H
#define ADVENT_DAY2_H

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

namespace day2 {
    using Vector = std::vector<long>;
    constexpr std::string_view kExampleFilePath = "day2/example.txt";
    constexpr std::string_view kPart1FilePath = "day2/part1.txt";

    class Level {
    public:
        explicit Level(Vector &&levels) : levels_{std::move(levels)} {
        }

        bool IsLevelSafePart1() const {
            Vector difference;
            if (levels_[1] - levels_[0] > 0) { // increasing
                std::transform(levels_.cbegin(), std::prev(levels_.cend()), std::next(levels_.cbegin()),
                               std::back_inserter(difference),
                               [](const auto &num1, const auto &num2) {
                                   return num2 - num1;
                               });
            } else {
                std::transform(levels_.cbegin(), std::prev(levels_.cend()), std::next(levels_.cbegin()),
                               std::back_inserter(difference),
                               [](const auto &num1, const auto &num2) {
                                   return num1 - num2;
                               });
            }
            bool is_max_difference_at_most_three = *std::max_element(difference.cbegin(), difference.cend()) <= 3;
            bool is_min_difference_at_least_one = *std::min_element(difference.cbegin(), difference.cend()) >= 1;
            return is_max_difference_at_most_three && is_min_difference_at_least_one;
        }

        bool IsForSureIncreasing() {
            return levels_[1] - levels_[0] > 0 && levels_[2] - levels_[1] > 0 && levels_[3] - levels_[2] > 0;
        }

        bool IsForSureDecreasing() {
            return levels_[1] - levels_[0] < 0 && levels_[2] - levels_[1] < 0 && levels_[3] - levels_[2] < 0;
        }

        bool IsLevelSafePart2() {
            return IsLevelSafePart2(levels_, true);
        }

        Vector GetLevels() {
            return levels_;
        }

    private:
        bool IsLevelSafePart2(Vector &levels, bool do_fallback) {
            Vector difference;
            if (IsForSureIncreasing()) { // increasing
                std::transform(levels.cbegin(), std::prev(levels.cend()), std::next(levels.cbegin()),
                               std::back_inserter(difference),
                               [](const auto &num1, const auto &num2) {
                                   return num2 - num1;
                               });
            } else if (IsForSureDecreasing()) {
                std::transform(levels.cbegin(), std::prev(levels.cend()), std::next(levels.cbegin()),
                               std::back_inserter(difference),
                               [](const auto &num1, const auto &num2) {
                                   return num1 - num2;
                               });
            } else {
                // try both and get the best result
                Vector try_increasing_difference{};
                std::transform(levels.cbegin(), std::prev(levels.cend()), std::next(levels.cbegin()),
                               std::back_inserter(try_increasing_difference),
                               [](const auto &num1, const auto &num2) {
                                   return num2 - num1;
                               });
                const auto count_increasing_problems = std::count_if(try_increasing_difference.cbegin(),
                                                                     try_increasing_difference.cend(),
                                                                     [](const auto &elem) { return elem < 0; });
                Vector try_decreasing_difference{};
                std::transform(levels.cbegin(), std::prev(levels.cend()), std::next(levels.cbegin()),
                               std::back_inserter(try_decreasing_difference),
                               [](const auto &num1, const auto &num2) {
                                   return num1 - num2;
                               });
                const auto count_decreasing_problems = std::count_if(try_decreasing_difference.cbegin(),
                                                                     try_decreasing_difference.cend(),
                                                                     [](const auto &elem) { return elem < 0; });
                if (count_increasing_problems < count_decreasing_problems) {
                    difference = std::move(try_increasing_difference);
                } else {
                    difference = std::move(try_decreasing_difference);
                }

            }
            auto max_difference_itr = std::max_element(difference.cbegin(), difference.cend());
            auto min_difference_itr = std::min_element(difference.cbegin(), difference.cend());

            bool is_min_difference_at_least_one = *min_difference_itr >= 1;
            bool is_max_difference_at_most_three = *max_difference_itr <= 3;

            bool fallback_worked = false;
            if (do_fallback)
            {
                if (!is_max_difference_at_most_three)
                {
                    std::cout << "max difference above 3" << std::endl;
                    fallback_worked = Fallback(levels, difference, max_difference_itr);
                }
                if (!is_min_difference_at_least_one && !fallback_worked)
                {
                    std::cout << "min difference at below 1" << std::endl;
                    fallback_worked = Fallback(levels, difference, min_difference_itr);
                }
            }
            return (is_max_difference_at_most_three && is_min_difference_at_least_one) ||
                   (do_fallback && fallback_worked);
        }

        bool Fallback(const Vector &levels, const Vector &difference, const auto itr) {
            const auto index = std::distance(difference.cbegin(), std::next(itr));
            std::cout << "Trying fallback for ";
            PrintVector(levels);
            std::cout << "\n";

            auto levels_without_first_element_of_difference = levels;
            levels_without_first_element_of_difference.erase(
                    levels_without_first_element_of_difference.cbegin() + index - 1);
            const bool fallback_worked_for_first_element_of_difference = IsLevelSafePart2(
                    levels_without_first_element_of_difference, false);
            if (fallback_worked_for_first_element_of_difference) {
                std::cout << "First element of difference worked ";
                PrintVector(levels_without_first_element_of_difference);
                std::cout << std::endl;
            }

            auto levels_without_second_element_of_difference = levels;
            levels_without_second_element_of_difference.erase(
                    levels_without_second_element_of_difference.cbegin() + index);
            const bool fallback_worked_for_second_element_of_difference = IsLevelSafePart2(
                    levels_without_second_element_of_difference, false);
            if (fallback_worked_for_second_element_of_difference) {
                std::cout << "Second element of difference worked ";
                PrintVector(levels_without_second_element_of_difference);
                std::cout << std::endl;
            }

            return fallback_worked_for_first_element_of_difference ||
                   fallback_worked_for_second_element_of_difference;
        }

        Vector levels_;
    };

    class Result {
    public:
        void IncreaseCounter() {
            ++counter_;
        }

        auto GetCounter() {
            return counter_;
        }

    private:
        long counter_{0};
    };

// report per line
// each report is a list of levels separated by spaces
// reactor safety systems only tolerate gradually increasing or decreasing levels
// it's also only safe if two adjacent levels differ by at least one or at most 3
    class FileParser {
    public:
        explicit FileParser(Text &&text) : part_1_result_{}, part_2_result_{} {
            for (auto &line: text) {
                auto numbers = GetNumbersInString(std::move(line));
                Level level{std::move(numbers)};
                if (level.IsLevelSafePart1()) {
                    part_1_result_.IncreaseCounter();
                }
                if (level.IsLevelSafePart2()) {
                    //setbuf(stdout, 0);
                    //std::cout << "safe: ";
                    //PrintVector(level.GetLevels());
                    //std::cout << "\n";
                    part_2_result_.IncreaseCounter();
                } else {
                    setbuf(stdout, 0);
                    std::cout << "unsafe: ";
                    PrintVector(level.GetLevels());
                    std::cout << "\n";
                }
            }
        }

        Result GetPart1Result() {
            return part_1_result_;
        }

        Result GetPart2Result() {
            return part_2_result_;
        }


    private:
        Result part_1_result_;
        Result part_2_result_;
    };


    void Run() {
        FileReader file_reader{kPart1FilePath};
        FileParser file_parser{file_reader.GetText()};
        // Part 1: How many reports are safe
        std::cout << "Part 1 - Number of safe reports: " << file_parser.GetPart1Result().GetCounter() << std::endl;
        std::cout << "Part 2 - Number of safe reports: " << file_parser.GetPart2Result().GetCounter() << std::endl;
    }

}
#endif //ADVENT_DAY2_H
