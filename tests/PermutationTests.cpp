#include "gtest/gtest.h"
#include "ColorPermutation.h"


TEST(PermutationTest, TestIdentityPerm) {
    RubikPerm ident;
    EXPECT_EQ(white, ident.nextColor(white));
    EXPECT_EQ(yellow, ident.nextColor(yellow));
    EXPECT_EQ(red, ident.nextColor(red));
    EXPECT_EQ(blue, ident.nextColor(blue));
    EXPECT_EQ(green, ident.nextColor(green));
    EXPECT_EQ(orange, ident.nextColor(orange));

    EXPECT_EQ(white, ident.prevColor(white));
    EXPECT_EQ(yellow, ident.prevColor(yellow));
    EXPECT_EQ(red, ident.prevColor(red));
    EXPECT_EQ(blue, ident.prevColor(blue));
    EXPECT_EQ(green, ident.prevColor(green));
    EXPECT_EQ(orange, ident.prevColor(orange));
}

TEST(PermutationTest, TestFwdBwdPerm) {
    RubikPerm::Func f({
        {white, yellow},
        {yellow, white},
        {red, blue},
        {blue, red},
        {green, orange},
        {orange, green}
    });

    RubikPerm fPerm;
    fPerm.setForward(f);

    EXPECT_EQ(white, fPerm.prevColor(fPerm.nextColor(white)));
    EXPECT_EQ(yellow, fPerm.prevColor(fPerm.nextColor(yellow)));
    EXPECT_EQ(red, fPerm.prevColor(fPerm.nextColor(red)));
    EXPECT_EQ(blue, fPerm.prevColor(fPerm.nextColor(blue)));
    EXPECT_EQ(green, fPerm.prevColor(fPerm.nextColor(green)));
    EXPECT_EQ(orange, fPerm.prevColor(fPerm.nextColor(orange)));
}
