#include "gtest/gtest.h"

#include "RubikBase.h"


TEST(RubikBaseTest, TestColorVectors) {
    EXPECT_EQ(green, getColorFromVeci(Vector3i({1, 0, 0})));
    EXPECT_EQ(blue, getColorFromVeci(Vector3i({-1, 0, 0})));
    EXPECT_EQ(white, getColorFromVeci(Vector3i({0, 1, 0})));
    EXPECT_EQ(yellow, getColorFromVeci(Vector3i({0, -1, 0})));
    EXPECT_EQ(red, getColorFromVeci(Vector3i({0, 0, 1})));
    EXPECT_EQ(orange, getColorFromVeci(Vector3i({0, 0, -1})));

    EXPECT_EQ(noColor, getColorFromVeci(Vector3i({2, 0, 0})));
    EXPECT_EQ(noColor, getColorFromVeci(Vector3i({1, 1, 0})));
    EXPECT_EQ(noColor, getColorFromVeci(Vector3i({1, 2, 3})));
}