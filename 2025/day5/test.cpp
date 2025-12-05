#include "day5.h"
#include <gtest/gtest.h>

TEST(Day5, Example)
{
    FileParser parser{ReadFile("2025/day5/example.txt")};
    CalculateValidIds calculate_valid_ids{parser.ViewValidIdRanges(), parser.ViewAvailableIds()};
    EXPECT_EQ(calculate_valid_ids.GetAvailableValidIds().size(), 3);
    EXPECT_EQ(calculate_valid_ids.GetAllValidIds(), 14);
}

TEST(Day5, Real)
{
    FileParser parser{ReadFile("2025/day5/real.txt")};
    CalculateValidIds calculate_valid_ids{parser.ViewValidIdRanges(), parser.ViewAvailableIds()};
    EXPECT_EQ(calculate_valid_ids.GetAvailableValidIds().size(), 828);
    EXPECT_EQ(calculate_valid_ids.GetAllValidIds(), 352681648086146);
}