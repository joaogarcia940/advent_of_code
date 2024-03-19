#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <regex>
#include <unordered_map>
#include <cassert>
#include "utils/utils.h"

class FileParserDay1 : public FileParser<FileParserDay1> // crtp / decorator
{
public:
    FileParserDay1 &Filter() override
    {
        assert(text_.begin() != text_.end());
        std::for_each(text_.begin(), text_.end(), [this](std::string &line)
                      {
        auto it = std::sregex_iterator(line.begin(), line.end(), pattern_);
        if (it != std::sregex_iterator())
        {
            line = (*it)[0].str();
        }
        else
        {
            std::cerr << "Unable to find first number" << std::endl;
        } });

        return *this;
    }

    FileParserDay1 &WithReversedText()
    {
        std::for_each(text_.begin(), text_.end(), [](std::string &line)
                      { return std::reverse(line.begin(), line.end()); });
        return *this;
    }

    FileParserDay1 &WithExtensiveNumberSubstitution(std::unordered_map<std::string, std::string> word_to_int)
    {
        std::for_each(text_.begin(), text_.end(), [&word_to_int](std::string &line)
                      {
                            if(word_to_int.find(line) != word_to_int.end())
                            {
                                    line = word_to_int[line];
                            } });
        return *this;
    }
};

Text MergeTextNumbers(const Text &first_numbers, const Text &last_numbers)
{
    Text merge{};
    if (first_numbers.size() != last_numbers.size())
    {
        std::cerr << "First and last number's sizes don't match" << std::endl;
        return merge;
    }

    for (size_t i = 0; i < first_numbers.size(); i++)
    {
        merge.push_back(first_numbers[i] + last_numbers[i]);
    }
    return merge;
}

int SumNumbersInText(Text text)
{
    int sum = 0;
    for_each(text.begin(), text.end(), [&sum](std::string line)
             { return sum += std::stoi(line); });
    return sum;
}
