//
// Created by CTW00728 on 12/3/2024.
//

#ifndef ADVENT_DAY1_H
#define ADVENT_DAY1_H

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

namespace day1 {
    using Group = std::unique_ptr<std::vector<long>>;

    constexpr std::string_view kExampleFilePath = "day1/example.txt";
    constexpr std::string_view kPart1FilePath = "day1/part1.txt";

    class FileParser {
    public:
        explicit FileParser(Text &&text) : group1_{std::make_unique<std::vector<long>>()},
                                           group2_{std::make_unique<std::vector<long>>()} {
            for (auto &line: text) {
                const auto numbers = GetNumbersInString(std::move(line));
                if (numbers.size() != 2) {
                    std::cout << "Problem reading the input: didn't find 2 numbers in the line";
                }
                if (group1_ == nullptr) {
                    std::cout << "group1_ is a nullptr" << std::endl;
                }
                if (group2_ == nullptr) {
                    std::cout << "group2_ is a nullptr" << std::endl;
                }
                group1_->push_back(numbers.front());
                group2_->push_back(numbers.back());
            }
        }

        auto GetGroup1() {
            return std::move(group1_);
        }

        auto GetGroup2() {
            return std::move(group2_);
        }

    private:
        Group group1_{};
        Group group2_{};
    };

    class Groups {
    public:
        Groups(Group &&group1, Group &&group2) : group1_{std::move(group1)},
                                                 group2_{std::move(group2)} {
            std::sort(group1_->begin(), group1_->end());
            std::sort(group2_->begin(), group2_->end());
        }

        long GetSumOfDifferences() {
            const auto difference = GetDifferenceVector();
            return std::reduce(difference.begin(), difference.end());
        }

        long GetSumOfSimilarity() {
            const auto similarity = GetSimilarityByNumber();
            return std::reduce(similarity.cbegin(), similarity.cend());
        }


    private:
        std::vector<long> GetDifferenceVector() {
            std::vector<long> difference;
            std::transform(group1_->cbegin(), group1_->cend(), group2_->cbegin(), std::back_inserter(difference),
                           [](const auto &num1, const auto &num2) {
                               return std::abs(num1 - num2);
                           });

            return difference;
        }

        std::vector<long> GetSimilarityByNumber() {
            std::vector<long> similarity;
            for (auto &num: *group1_) {
                long num_count_on_group2 = std::count(group2_->cbegin(), group2_->cend(), num);
                similarity.push_back(num * num_count_on_group2);
            }
            return similarity;
        }


        Group group1_;
        Group group2_;
    };



// list of locations -> empty
// location ID
// two groups search in the office by its location
// problem: when holding lists side by side it's clear they are not the same
// goal: help the historians reconcile their lists



void Run() {
    FileReader file_reader{kPart1FilePath};
    FileParser file_parser{file_reader.GetText()};
    Groups groups{file_parser.GetGroup1(), file_parser.GetGroup2()};
    std::cout << "Part1: Sum of differences between groups: " << groups.GetSumOfDifferences() << std::endl;
    std::cout << "Part2: Similarity Score: " << groups.GetSumOfSimilarity() << std::endl;
}

}

#endif //ADVENT_DAY1_H
