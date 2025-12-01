#include "day1.h"
#include <gtest/gtest.h>

enum Type
{
    Scenario,
    Part1,
    Part2,
};

class Day1Fixture : public ::testing::TestWithParam<std::tuple<Text, Counter, Counter>>
{
  protected:
    Day1Fixture() : parser_(std::get<Scenario>(GetParam())), rotator_(parser_.ViewCommands()) {}
    FileParser parser_;
    DialRotator rotator_;
};

INSTANTIATE_TEST_SUITE_P(Day1Tests,
                         Day1Fixture,
                         ::testing::Values(std::make_tuple(Text{"L50", "R200"}, 2, 3),
                                           std::make_tuple(Text{"R50", "L200"}, 2, 3),
                                           std::make_tuple(Text{"L50", "R199"}, 1, 2),
                                           std::make_tuple(Text{"R50", "L199"}, 1, 2),
                                           std::make_tuple(Text{"R50", "R50", "L100"}, 1, 2),
                                           std::make_tuple(Text{"L60", "L50", "R30", "R80"}, 0, 2)));

TEST_P(Day1Fixture, TestDay1)
{
    EXPECT_EQ(rotator_.GetCountFinishedIn0(), std::get<Part1>(GetParam()));
    EXPECT_EQ(rotator_.GetCountPassed0(), std::get<2>(GetParam()));
}

TEST(Day1RealInput, Part1)
{
    FileParser parser{ReadFile("day1/real.txt")};
    DialRotator rotator{parser.ViewCommands()};
    EXPECT_EQ(rotator.GetCountFinishedIn0(), 1177);
}

TEST(Day1RealInput, Part2)
{
    FileParser parser{ReadFile("day1/real.txt")};
    DialRotator rotator{parser.ViewCommands()};
    EXPECT_GT(rotator.GetCountPassed0(), 6738);
    EXPECT_LT(rotator.GetCountPassed0(), 7217);
}
