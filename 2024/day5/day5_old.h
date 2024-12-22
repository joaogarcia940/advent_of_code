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
#include <sstream>
#include <algorithm>
#include <numeric>
#include <set>
#include "../utils/file_reader.h"
#include "../utils/numbers_in_string.h"
#include "../utils/print_vector.h"

namespace day5 {
    constexpr std::string_view example = "day5/example.txt";
    constexpr std::string_view real = "day5/real.txt";
    constexpr std::string_view test = "day5/test.txt";

    struct PageOrderingRule {
        long x{};
        long y{};
    };

    std::ostream &operator<<(std::ostream &os, const PageOrderingRule &page_ordering_rule) {
        os << "(x: " << page_ordering_rule.x << ", y: " << page_ordering_rule.y << ")";
        return os;
    }

    using PageOrderingRules = std::vector<PageOrderingRule>;

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
                    page_ordering_rules_.push_back({line_updates_vec.front(), line_updates_vec.back()});
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
        std::vector<long> GetNumbersInString(const std::string &str, std::regex &&regex) {
            auto it = std::sregex_iterator{str.cbegin(), str.cend(), regex};
            auto end = std::sregex_iterator();
            std::vector<long> numbers;
            for (; it != end; ++it) {
                numbers.push_back(stol((*it).str()));
            }
            return numbers;
        }

        PageOrderingRules page_ordering_rules_;
        std::vector<std::vector<long>> updates_per_manual_{};

    };

    enum class Prio {
        X,
        Y,
        Invalid
    };

    std::string ToString(const Prio prio) {
        switch (prio) {
            case Prio::X:
                return "X";
            case Prio::Y:
                return "Y";
            case Prio::Invalid:
                return "Invalid";
        }
    }

    struct PrioNode {
        long page{};
        Prio prio{};
        std::shared_ptr<PrioNode> next_node{nullptr};
    };

    using PrioNodePtr = std::shared_ptr<PrioNode>;
    using PrioNodesPtrs = std::vector<PrioNodePtr >;

    class PrioLinkedList {
    public:
        explicit PrioLinkedList(PageOrderingRules &&page_ordering_rules) : heads_{} {
            for (auto &page_ordering_rule: page_ordering_rules) {
                // find x page in list
                if (heads_.empty()) {
                    const auto next_node =
                            std::make_shared<PrioNode>(PrioNode{page_ordering_rule.y, Prio::Y, nullptr});
                    heads_.push_back(std::make_shared<PrioNode>(
                            PrioNode{page_ordering_rule.x, Prio::X, next_node}));
                    continue;
                }
                if (auto heads_in_lists = FindAllOfOrderingPagesInAllLists(
                            page_ordering_rule); not heads_in_lists.empty()) {
                    OrderInLists(page_ordering_rule, std::move(heads_in_lists));
                } else {
                    const auto next_node =
                            std::make_shared<PrioNode>(PrioNode{page_ordering_rule.y, Prio::Y, nullptr});
                    heads_.push_back(std::make_shared<PrioNode>(
                            PrioNode{page_ordering_rule.x, Prio::X, next_node}));
                }
            }
            PrintLists();
        }

        void OrderInLists(const PageOrderingRule &page_ordering_rule, PrioNodesPtrs&& heads) {
            if (heads.size() == 1)
            {
                OrderInList(page_ordering_rule, heads.front());
                return;
            }
            for (auto &head: heads) {
                OrderInList(page_ordering_rule, head); // does this change heads_? yes
            }
            MergeLists(page_ordering_rule, heads);
        }

        void MergeLists(const PageOrderingRule &page_ordering_rule, const PrioNodesPtrs& heads)
        {
            if (heads.size() <= 1)
            {
                std::cerr << "Trying to merge with less than 1 list available" << std::endl;
                return;
            }

            const auto& head_to_merge_to = heads.front();
            const auto& node_to_merge_to_x = FindAnyPageInList(page_ordering_rule.x, heads.front());
            const auto& node_to_merge_to_y = FindAnyPageInList(page_ordering_rule.y, heads.front());
            const auto type_of_page_ordering_rule_in_head_to_merge_to = Prio::Invalid;

            for (auto it = std::next(heads.begin()); it != heads.end(); ++it)
            {
                const auto& head = *it;
                const auto& node_to_merge_x = FindAnyPageInList(page_ordering_rule.x, head);
                const auto& node_to_merge_y = FindAnyPageInList(page_ordering_rule.y, head);

                if (node_to_merge_to_x && node_to_merge_x)
                {

                }

            }


            if (node_to_merge_to_x)
            {

            }
        }

        PrioNodePtr OrderInList(const PageOrderingRule &page_ordering_rule, const PrioNodePtr& head) {
            if (const auto node = FindAnyPageInList(page_ordering_rule.y, head)) {
                if (node == head) {
                    return InsertAtTheBeginningAndReturnNewHead(head, page_ordering_rule.x, Prio::X);
                }
                InsertBeforeNode(head, node,
                                 std::make_shared<PrioNode>(PrioNode{page_ordering_rule.x, Prio::X, nullptr}));
                return head;
            }
            if (const auto node = FindAnyPageInList(page_ordering_rule.x, head)) {
                InsertAfterNode(head, node,
                                std::make_shared<PrioNode>(PrioNode{page_ordering_rule.y, Prio::Y, nullptr}));
            }
            return head;
        }

        [[nodiscard]] PrioNodesPtrs FindAllOfOrderingPagesInAllLists(const PageOrderingRule &pages) const {
            PrioNodesPtrs heads_found;
            for (auto &head: heads_) {
                const auto head_found = FindAnyOfOrderingPagesInList(pages, head);
                if (head_found) {
                    heads_found.push_back(head_found);
                }
            }
            return heads_found;
        }

        PrioNodePtr
        FindAnyOfOrderingPagesInList(const PageOrderingRule &pages, const PrioNodePtr &head) const {
            auto current = head;
            while (current) {
                if (current->page == pages.x || current->page == pages.y) {
                    return current;
                }

                current = current->next_node;
            }
            return nullptr;
        }

        PrioNodePtr
        FindAnyPageInList(const long page, const PrioNodePtr &head) const {
            auto current = head;
            while (current) {
                if (current->page == page) {
                    return current;
                }
                current = current->next_node;
            }
            return nullptr;
        }

        void PrintLists() {
            for (auto &head: heads_) {
                PrintList(head);
            }
        }

        void PrintList(const PrioNodePtr &head) const {
            auto current = head; // Get raw pointer for traversal
            while (current) {
                std::cout << current->page << " " << ToString(current->prio) << " -> ";
                current = current->next_node; // Move to the next node
            }
            std::cout << "nullptr" << std::endl;
        }

    private:
        PrioNodePtr
        InsertAtTheBeginningAndReturnNewHead(const PrioNodePtr &head, const long page, const Prio prio) {
            return std::make_shared<PrioNode>(PrioNode{page, prio, head});
        }

        void InsertBeforeNode(const PrioNodePtr &head, const PrioNodePtr &node_after,
                              const PrioNodePtr &node_to_insert) {
            if (node_after == head) {
                std::cerr << "Can't insert before head" << std::endl;
            }
            auto before = head;
            auto current = head->next_node;
            while (current) {
                if (before->next_node != current) {
                    std::cerr << "Problem inserting node before" << std::endl;
                }
                if (current == node_after) {
                    before->next_node = node_to_insert;
                    node_to_insert->next_node = node_after;
                    return;
                }
                before = before->next_node;
                current = current->next_node;
            }
            std::cerr << "Problem inserting node before: reached end of iteration performing fallback" << std::endl;
            before->next_node = node_to_insert;
            node_to_insert->next_node = nullptr;
        }

        void InsertAfterNode(const PrioNodePtr &head, const PrioNodePtr &node_before,
                             const PrioNodePtr &node_to_insert) {
            if (node_before == head) {
                node_before->next_node = node_to_insert;
                return;
            }
            auto before = head;
            auto current = head->next_node;
            while (current) {
                if (before->next_node != current) {
                    std::cerr << "Problem inserting node before" << std::endl;
                }
                if (before == node_before) {
                    before->next_node = node_to_insert;
                    node_to_insert->next_node = current;
                    return;
                }
                before = before->next_node;
                current = current->next_node;
            }
            before->next_node = node_to_insert;
            node_to_insert->next_node = nullptr;
        }

        std::vector<std::shared_ptr<PrioNode>> heads_;
    };

    void Run() {
        FileReader file_reader{test};
        FileParser file_parser{file_reader.GetText()};
        PrioLinkedList list{file_parser.GetPageOrderingRules()};
    }
}

#endif //ADVENT_DAY5_H
