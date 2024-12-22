//
// Created by CTW00728 on 12/3/2024.
//

#ifndef ADVENT_PRINT_VECTOR_H
#define ADVENT_PRINT_VECTOR_H

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

template<typename T>
void PrintVector(T &&vector) {
    for_each(vector.cbegin(), vector.cend(), [](const auto &element) { std::cout << element << " "; });
}

#endif //ADVENT_PRINT_VECTOR_H
