#include "day4.h"
#include <gtest/gtest.h>

enum Type
{
    Scenario,
    Part1,
    Part2,
};

class Day4Fixture : public ::testing::TestWithParam<std::tuple<Text, std::size_t, std::size_t>>
{
  protected:
    Day4Fixture() : parser_(std::get<Scenario>(GetParam())), calculator_(parser_.GetMap()) {}
    FileParser parser_;
    CalculateAccessesByForklift calculator_;
};

INSTANTIATE_TEST_SUITE_P(Day4Tests,
                         Day4Fixture,
                         ::testing::Values(std::make_tuple(Text{"@..@",  //
                                                                "....",  //
                                                                ".@..",  //
                                                                "...."},
                                                           3,
                                                           3),
                                           std::make_tuple(Text{"@..@",  //
                                                                "....",  //
                                                                ".@..",  //
                                                                "...."},
                                                           3,
                                                           3),
                                           std::make_tuple(Text{"@@.@",  //
                                                                "@@@.",  //
                                                                "@@@.",  //
                                                                "...."},
                                                           4,
                                                           9)));
TEST_P(Day4Fixture, TestDay4)
{
    EXPECT_EQ(calculator_.GetAccessesCounterPart1(), std::get<Part1>(GetParam()));
    EXPECT_EQ(calculator_.GetAccessesCounterPart2(), std::get<Part2>(GetParam()));
}

TEST(Day4, Example)
{
    FileParser parser{ReadFile("2025/day4/example.txt")};
    CalculateAccessesByForklift calculator_{parser.GetMap()};
    EXPECT_EQ(calculator_.GetAccessesCounterPart1(), 13);
    EXPECT_EQ(calculator_.GetAccessesCounterPart2(), 43);
}

TEST(Day4, Real)
{
    FileParser parser{ReadFile("2025/day4/real.txt")};
    CalculateAccessesByForklift calculator_{parser.GetMap()};
    EXPECT_EQ(calculator_.GetAccessesCounterPart1(), 1543);
    EXPECT_EQ(calculator_.GetAccessesCounterPart2(), 9038);
}