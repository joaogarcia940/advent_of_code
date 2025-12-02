#include "day2.h"
#include <gtest/gtest.h>

enum Type
{
    Scenario,
    Part1,
    Part2,
};

class Day2Fixture : public ::testing::TestWithParam<std::tuple<Text, std::size_t, std::size_t>>
{
  protected:
    Day2Fixture() : parser_(std::get<Scenario>(GetParam())), detector_(parser_.ViewRanges()) {}
    FileParser parser_;
    InvalidIdsDetector detector_;
};

INSTANTIATE_TEST_SUITE_P(Day2Tests,
                         Day2Fixture,
                         ::testing::Values(std::make_tuple(Text{"0-1"}, 0, 0),
                                           std::make_tuple(Text{"11-22"}, 2, 2),
                                           std::make_tuple(Text{"11-22,33-44"}, 4, 4),
                                           std::make_tuple(Text{"100100100-100100101"}, 0, 1),
                                           std::make_tuple(Text{"10011001-10011001"}, 1, 1)));
TEST_P(Day2Fixture, TestDay2)
{
    EXPECT_EQ(detector_.GetCountInvalidIdsPart1(), std::get<Part1>(GetParam()));
    EXPECT_EQ(detector_.GetCountInvalidIdsPart2(), std::get<Part2>(GetParam()));
}

TEST(Day2, Example)
{
    FileParser parser{ReadFile("2025/day2/example.txt")};
    InvalidIdsDetector detector{parser.ViewRanges()};
    EXPECT_EQ(detector.GetCountInvalidIdsPart1(), 8);
    EXPECT_EQ(detector.GetSumInvalidIdsPart1(), 1227775554);
    EXPECT_EQ(detector.GetCountInvalidIdsPart2(), 13);
    EXPECT_EQ(detector.GetSumInvalidIdsPart2(), 4174379265);
}

TEST(Day2, Real)
{
    FileParser parser{ReadFile("2025/day2/real.txt")};
    InvalidIdsDetector detector{parser.ViewRanges()};
    EXPECT_EQ(detector.GetCountInvalidIdsPart1(), 748);
    EXPECT_EQ(detector.GetSumInvalidIdsPart1(), 30599400849);
    EXPECT_EQ(detector.GetCountInvalidIdsPart2(), 821);
    EXPECT_EQ(detector.GetSumInvalidIdsPart2(), 46270373595);
}
