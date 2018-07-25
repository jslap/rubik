#include "gtest/gtest.h"
#include "ColorPermutation.h"

class PermBaseTest : public testing::Test {
protected:
    virtual void SetUp() {}

    void checkPerm(const RubikPerm& fPerm)
    {
        auto iterArray = {1,2,3, 4, 5, 6};
        for (auto col: RubikBase::RubikColors())
        {
            for (auto iter: iterArray)
            {
                EXPECT_EQ(col, fPerm.prevColor(fPerm.nextColor(col, iter), iter));
            }
        }
    }
};

TEST(ColorPermutationTest, TestIdentityPerm) {
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


TEST_F(PermBaseTest, TestFwdBwdPerm) {
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

    EXPECT_EQ(yellow, fPerm.nextColor(white));
    EXPECT_EQ(white, fPerm.nextColor(white, 2));
    EXPECT_EQ(yellow, fPerm.prevColor(white));
    EXPECT_EQ(white, fPerm.prevColor(white, 2));


    checkPerm(fPerm);

}

TEST_F(PermBaseTest, TestFwdBwdPerm2) {
    RubikPerm::Func f({
        {white, white},
        {yellow, red},
        {red, blue},
        {blue, green},
        {green, orange},
        {orange, yellow}
    });

    RubikPerm originalPermute;
    originalPermute.setForward(f);

    auto checkThisPermute = [&] (const RubikPerm& fPerm) 
    {
        EXPECT_EQ(white, fPerm.nextColor(white));
        EXPECT_EQ(white, fPerm.prevColor(white));

        EXPECT_EQ(red, fPerm.nextColor(yellow));
        EXPECT_EQ(blue, fPerm.nextColor(yellow, 2));
        EXPECT_EQ(green, fPerm.nextColor(yellow, 3));
        EXPECT_EQ(orange, fPerm.nextColor(yellow, 4));
        EXPECT_EQ(yellow, fPerm.nextColor(yellow, 5));

        EXPECT_EQ(yellow, fPerm.prevColor(red));
        EXPECT_EQ(orange, fPerm.prevColor(red, 2));
        EXPECT_EQ(green, fPerm.prevColor(red, 3));
        EXPECT_EQ(blue, fPerm.prevColor(red, 4));
        EXPECT_EQ(red, fPerm.prevColor(red, 5));
    };

    checkThisPermute(originalPermute);
    RubikPerm copyConstructPermute(originalPermute);
    checkThisPermute(copyConstructPermute);
    RubikPerm assignedPermute;
    assignedPermute = originalPermute;
    checkThisPermute(assignedPermute);

    checkPerm(originalPermute);

}
