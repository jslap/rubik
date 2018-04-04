#include "gtest/gtest.h"
#include "ColorPermMap.h"

TEST(ColorPermMapTest, SameFacePermutationMapTest)
{
    const std::vector< RubikColor > AllColor({white, yellow, red, green, orange, blue});
    for (RubikColor c : AllColor)
    {
        EXPECT_EQ(PermutationMap::PermutationBySideRotation(c).nextColor(c), c) << "Rotating CW around "   << c << " should keep " << c << " steady.";
        EXPECT_EQ(PermutationMap::PermutationBySideRotation(c).prevColor(c), c) << "Rotating CCW around "  << c << " should keep " << c << " steady.";
    }

}

TEST(ColorPermMapTest, OposingPermutationMapTest)
{
    const std::vector<std::pair<RubikColor, RubikColor>> OposingColor({
        {white, yellow},
        {orange, red},
        {blue, green}
    });
    for (auto opPair : OposingColor)
    {
        RubikColor c1 = opPair.first;
        RubikColor c2 = opPair.second;
        EXPECT_EQ(PermutationMap::PermutationBySideRotation(c1).nextColor(c2), c2) << "Rotating CW around "  << c1 << " should keep " << c2 << " steady.";
        EXPECT_EQ(PermutationMap::PermutationBySideRotation(c1).prevColor(c2), c2) << "Rotating CCW around " << c1 << " should keep " << c2 << " steady.";
        EXPECT_EQ(PermutationMap::PermutationBySideRotation(c2).nextColor(c1), c1) << "Rotating CW around "  << c2 << " should keep " << c1 << " steady.";
        EXPECT_EQ(PermutationMap::PermutationBySideRotation(c2).prevColor(c1), c1) << "Rotating CCW around " << c2 << " should keep " << c1 << " steady.";
    }
}

TEST(ColorPermMapTest, SomePermutationMapTest)
{
    // (rotation-face, src-color, dst-color)
    const std::vector<std::tuple<RubikColor, RubikColor, RubikColor>> ColorToTest({
        {white, red, blue},
        {green, red, white},
        {blue, orange, white}
    });

    for (auto testInstance : ColorToTest)
    {
        RubikColor rotColor = std::get<0>(testInstance);
        RubikColor srcColor = std::get<1>(testInstance);
        RubikColor dstColor = std::get<2>(testInstance);

        EXPECT_EQ(PermutationMap::PermutationBySideRotation(rotColor).nextColor(srcColor), dstColor)
            << "Rotating CW around "  << rotColor << " should bring " << srcColor << " to " << dstColor;
        EXPECT_EQ(PermutationMap::PermutationBySideRotation(rotColor).prevColor(dstColor), srcColor)
            << "Rotating CCW around "  << rotColor << " should bring " << dstColor << " to " << srcColor;
    }
}
