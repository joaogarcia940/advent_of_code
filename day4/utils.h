#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <regex>
#include <unordered_map>
#include <cassert>
#include <limits>
#include <numeric>
#include <algorithm>
#include <map>
#include <math.h>
#include "utils/utils.h"

class Game
{
public:
    int GetPoints()
    {
        return points_;
    }

    void CountPoints(std::vector<int> winning_card, std::vector<int> hand_card)
    {
        const auto winning_numbers = GetIntersections(winning_card, hand_card);
        if (winning_numbers.empty())
        {
            return;
        }
        else
        {
            points_ += pow(2, winning_numbers.size() - 1);
        }
    }

    void FillCards(std::vector<int> winning_card, std::vector<int> hand_card, const size_t card_num)
    {
        main_cards_to_card_copies_original_[card_num] = std::vector<size_t>{};
        const auto num_of_won_sequential_cards = GetIntersections(winning_card, hand_card).size();
        for (size_t i = card_num + 1; i < card_num + num_of_won_sequential_cards + 1; i++)
        {
            main_cards_to_card_copies_original_[card_num].push_back(i);
        }
    }

    uint64_t CountCards()
    {
        FillEscalatedMap();

        uint64_t count = 0;
        for_each(main_cards_to_card_copies_escalated_.begin(), main_cards_to_card_copies_escalated_.end(),
                 [this, &count](const std::pair<size_t, size_t> &card)
                 {
                     ++count;
                     count += main_cards_to_card_copies_original_[card.first].size()*card.second;
                 });
        return count;
    }

private:
    std::vector<int> GetIntersections(std::vector<int> &a, std::vector<int> &b)
    {
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        std::vector<int> intersections;
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(intersections));
        return intersections;
    }

    void FillEscalatedMap()
    {
        for_each(main_cards_to_card_copies_original_.begin(), main_cards_to_card_copies_original_.end(),
                 [this](const std::pair<size_t, std::vector<size_t>> &card)
                 {
                    ++main_cards_to_card_copies_escalated_[card.first];
                    FillEscalatedMap(card.second, 0U);
                    });
    }

    void FillEscalatedMap(const std::vector<size_t> &card_copies, size_t i)
    {
        if (i >= card_copies.size())
        {
            return;
        }
        //const auto &original_copies = main_cards_to_card_copies_original_[card_copies[i]];

        ++main_cards_to_card_copies_escalated_[card_copies[i]];
        return FillEscalatedMap(card_copies, ++i);
    }

    int points_ = 0;
    std::map<size_t, std::vector<size_t>> main_cards_to_card_copies_original_;
    std::map<size_t, size_t> main_cards_to_card_copies_escalated_;
};

class FileParserDay4 : public FileParser<FileParserDay4> // crtp / decorator
{
public:
    FileParserDay4 &Filter() override
    {
        assert(text_.begin() != text_.end());
        size_t card_num = 1;
        for (auto line : text_)
        {
            auto line_without_card_id = line.substr(line.find(':') + 1, line.length());
            auto pipe_pos = line_without_card_id.find('|');
            auto first_half = line_without_card_id.substr(0, pipe_pos);
            auto second_half = line_without_card_id.substr(pipe_pos + 1);

            int num;
            std::vector<int> winning_card;
            std::vector<int> hand_card;

            std::istringstream iss1(first_half);
            while (iss1 >> num)
            {
                winning_card.push_back(num);
            }
            std::istringstream iss2(second_half);
            while (iss2 >> num)
            {
                hand_card.push_back(num);
            }
            // part 1
            game_.CountPoints(winning_card, hand_card);

            // part 2
            std::istringstream iss_card_num(line.substr(4, line.find(':')));
            while (iss_card_num >> num)
            {
                card_num = num;
            }
            game_.FillCards(winning_card, hand_card, card_num);
        }
        return *this;
    }

    Game GetGame()
    {
        return game_;
    }

private:
    Game game_;
};
