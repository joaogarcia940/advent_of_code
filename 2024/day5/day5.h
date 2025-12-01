//
// Created by CTW00728 on 12/8/2024.
//

#ifndef ADVENT_DAY5_H
#define ADVENT_DAY5_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <string_view>
#include <optional>
#include <queue>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <set>
#include <regex>
#include "../utils/file_reader.h"
#include "../utils/numbers_in_string.h"
#include "../utils/print_vector.h"

namespace day5 {
    constexpr std::string_view example = "day5/example.txt";
    constexpr std::string_view real = "day5/real.txt";
    constexpr std::string_view test = "day5/test.txt";

    using Page = long;
    using Pages = std::vector<Page>;
    using PageOrderingRule = std::pair<Page, std::set<Page>>;
    using PageOrderingRules = std::unordered_map<Page, std::set<Page>>;
    using UpdatesPerManual = std::vector<Pages>;

    class FileParser {
    public:
        explicit FileParser(Text &&text) {
            for (auto &line: text) {
                auto update_regex = std::regex(R"(\d+\|\d+)");
                auto it = std::sregex_iterator{line.cbegin(), line.cend(), update_regex};
                if (it != std::sregex_iterator()) {
                    const auto line_updates_vec = GetNumbersInString(line, std::regex(R"(\d+)"));
                    if (line_updates_vec.size() != 2) {
                        std::cerr << "Error decoding file: updates" << std::endl;
                    }
                    page_ordering_rules_[line_updates_vec.front()].insert(line_updates_vec.back());
                } else {
                    const auto updates = GetNumbersInString(line, std::regex(R"(\d+)"));
                    if (!updates.empty()) {
                        updates_per_manual_.emplace_back(updates);
                    }
                }
            }
        }

        auto GetPageOrderingRules() {
            return page_ordering_rules_;
        }

        auto GetUpdatesPerManual() {
            return updates_per_manual_;
        }

    private:
        std::vector<Page> GetNumbersInString(const std::string &str, std::regex &&regex) {
            auto it = std::sregex_iterator{str.cbegin(), str.cend(), regex};
            auto end = std::sregex_iterator();
            std::vector<Page> numbers;
            for (; it != end; ++it) {
                numbers.push_back(stol((*it).str()));
            }
            return numbers;
        }

        PageOrderingRules page_ordering_rules_;
        UpdatesPerManual updates_per_manual_{};
    };

    class UpdateSolver {
    public:
        UpdateSolver(PageOrderingRules &&page_ordering_rules, UpdatesPerManual &&updates_per_manual)
                : page_ordering_rules_{page_ordering_rules}, updates_per_manual_{updates_per_manual} {}

        bool IsUpdateCorrect(const Pages &update_pages) const {
            return std::is_sorted(update_pages.cbegin(), update_pages.cend(),
                                  [&page_ordering_rules = page_ordering_rules_](const auto page_1, const auto page_2) {
                                      return page_ordering_rules.find(page_1) != page_ordering_rules.cend() &&
                                             page_ordering_rules.at(page_1).count(page_2) > 0;
                                  });
        }

        long Part1() const {
            long result{0};
            for (auto &update_pages: updates_per_manual_) {
                if (IsUpdateCorrect(update_pages)) {
                    result += update_pages[update_pages.size() / 2];
                }
            }
            return result;
        }

        long Part2() {
            long result{0};
            for (auto &update_pages: updates_per_manual_) {
                if (not IsUpdateCorrect(update_pages)) {
                    std::sort(update_pages.begin(), update_pages.end(),
                              [&page_ordering_rules = page_ordering_rules_](const auto page_1, const auto page_2) {
                                  return page_ordering_rules.find(page_1) != page_ordering_rules.cend() &&
                                         page_ordering_rules.at(page_1).count(page_2) > 0;
                              });
                    result += update_pages[update_pages.size() / 2];
                }
            }
            return result;
        }


    private:
        PageOrderingRules page_ordering_rules_;
        UpdatesPerManual updates_per_manual_;
    };


    void Run() {
        FileReader file_reader{real};
        FileParser file_parser{file_reader.GetText()};
        UpdateSolver update_solver{file_parser.GetPageOrderingRules(), file_parser.GetUpdatesPerManual()};
        std::cout << "Part 1: " << update_solver.Part1() << std::endl;
        std::cout << "Part 2: " << update_solver.Part2() << std::endl;
    }
}

#endif //ADVENT_DAY5_H
