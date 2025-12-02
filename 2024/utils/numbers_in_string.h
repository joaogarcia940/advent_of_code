//
// Created by CTW00728 on 12/3/2024.
//

#ifndef ADVENT_NUMBERS_IN_STRING_H
#define ADVENT_NUMBERS_IN_STRING_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <string_view>
#include <optional>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iostream>

static std::vector<long> GetNumbersInString(std::string &&line) {
    std::vector<long> numbers;
    std::istringstream stream(line);
    std::string token;

    // Read each token and check if it's a number
    while (stream >> token) {
        try {
            numbers.push_back(std::stol(token));  // Convert to long and add to the list
        } catch (const std::invalid_argument &) {
            // Not a number, ignore
        } catch (const std::out_of_range &) {
            std::cerr << "Number out of range: " << token << std::endl;
        }
    }

    return numbers;
}

#endif //ADVENT_NUMBERS_IN_STRING_H
