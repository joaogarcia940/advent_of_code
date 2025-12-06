#include "day6.h"
#include <gtest/gtest.h>

TEST(Day6, Example)
{
    const auto text = ReadFile("2025/day6/example.txt");

    Day6Part1 part_1{text};
    EXPECT_EQ(part_1.GetSolution(), 4277556);

    Day6Part2 part_2{text};
    EXPECT_EQ(part_2.GetSolution(), 3263827);
}

TEST(Day6, Real)
{
    const auto text = ReadFile("2025/day6/real.txt");

    Day6Part1 part_1{text};
    EXPECT_EQ(part_1.GetSolution(), 5552221122013);

    Day6Part2 part_2{text};
    EXPECT_EQ(part_2.GetSolution(), 11371597126232);
}