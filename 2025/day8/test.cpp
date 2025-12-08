#include "day8.h"
#include <gtest/gtest.h>

TEST(Day8, Example)
{
    FileParser parser{ReadFile("2025/day8/example.txt")};
    
    const ShortestConnections part_1{parser.GetPoints(), 10};
    EXPECT_EQ(part_1.GetPart1Result(), 40);
    
    const ShortestConnections part_2{parser.GetPoints(), std::numeric_limits<Number>::max()};
    EXPECT_EQ(part_2.GetPart2Result(), 25272);
}

TEST(Day8, Real)
{  
    FileParser parser{ReadFile("2025/day8/real.txt")};

    const ShortestConnections part_1{parser.GetPoints(), 1000};
    EXPECT_EQ(part_1.GetPart1Result(), 29406);
    
    const ShortestConnections part_2{parser.GetPoints(), std::numeric_limits<Number>::max()};
    EXPECT_EQ(part_2.GetPart2Result(), 7499461416);
}