#include "day7.h"
#include <gtest/gtest.h>

enum TestType
{
    Scenario,
    Part1,
    Part2,
};

class Day7Fixture : public ::testing::TestWithParam<std::tuple<Text, std::size_t, std::size_t>>
{
  protected:
    Day7Fixture() : simulator_(std::get<Scenario>(GetParam())) {}
    BeamSimulator simulator_;
};

INSTANTIATE_TEST_SUITE_P(Day7Tests,
                         Day7Fixture,
                         ::testing::Values(std::make_tuple(Text{"..S..",  //
                                                                ".....",  //
                                                                "..^..",  //
                                                                "....."},
                                                           1,
                                                           2),
                                           std::make_tuple(Text{"..S..",  //
                                                                ".....",  //
                                                                "..^..",  //
                                                                ".^.^.",  //
                                                                "....."},
                                                           3,
                                                           4),
                                           std::make_tuple(Text{"...S....",  //
                                                                "........",  //
                                                                "...^....",  //
                                                                "..^.^.^.",  //
                                                                "...^....",  //
                                                                "........"},
                                                           4,
                                                           6)));
TEST_P(Day7Fixture, TestDay7)
{
    EXPECT_EQ(simulator_.GetCountSplits(), std::get<Part1>(GetParam()));
    EXPECT_EQ(simulator_.GetCountTimelines(), std::get<Part2>(GetParam()));
}

TEST(Day7, Example)
{
    BeamSimulator simulator_{ReadFile("2025/day7/example.txt")};
    EXPECT_EQ(simulator_.GetCountSplits(), 21);
    EXPECT_EQ(simulator_.GetCountTimelines(), 40);
}

TEST(Day7, Real)
{
    BeamSimulator simulator_{ReadFile("2025/day7/real.txt")};
    EXPECT_EQ(simulator_.GetCountSplits(), 1516);
    EXPECT_EQ(simulator_.GetCountTimelines(), 1393669447690);
}