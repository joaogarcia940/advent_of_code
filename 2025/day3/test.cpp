#include <gtest/gtest.h>
#include "day3.h"


enum Type
{
    Scenario,
    Part1,
    Part2,
};

class Day3Fixture : public ::testing::TestWithParam<std::tuple<Text, std::size_t, std::size_t>>
{
  protected:
    Day3Fixture() : parser_(std::get<Scenario>(GetParam())), part_1_(parser_.ViewBattery(), kPart1), part_2_(parser_.ViewBattery(), kPart2) {}
    FileParser parser_;
    HighestJoltage part_1_;
    HighestJoltage part_2_;
};

INSTANTIATE_TEST_SUITE_P(Day3Tests,
                         Day3Fixture,
                         ::testing::Values(std::make_tuple(Text{"987654321111111"}, 98, 987654321111),
                                           std::make_tuple(Text{"811111111111119"}, 89, 811111111119),
                                           std::make_tuple(Text{"234234234234278"}, 78, 434234234278),
                                           std::make_tuple(Text{"818181911112111"}, 92, 888911112111)));
TEST_P(Day3Fixture, TestDay3)
{
    EXPECT_EQ(part_1_.GetHighestJoltageCounter(), std::get<Part1>(GetParam()));
    EXPECT_EQ(part_2_.GetHighestJoltageCounter(), std::get<Part2>(GetParam()));
}

TEST(Day3, Example)
{
    FileParser parser{ReadFile("2025/day3/example.txt")};
    HighestJoltage part_1{parser.ViewBattery(), kPart1};
    HighestJoltage part_2{parser.ViewBattery(), kPart2};
    EXPECT_EQ(part_1.GetHighestJoltageCounter(), 357);
    EXPECT_EQ(part_2.GetHighestJoltageCounter(), 3121910778619);
}

TEST(Day3, Real)
{
    FileParser parser{ReadFile("2025/day3/real.txt")};
    HighestJoltage part_1{parser.ViewBattery(), kPart1};
    HighestJoltage part_2{parser.ViewBattery(), kPart2};
    EXPECT_EQ(part_1.GetHighestJoltageCounter(), 17452);
    EXPECT_EQ(part_2.GetHighestJoltageCounter(), 173300819005913);
}
