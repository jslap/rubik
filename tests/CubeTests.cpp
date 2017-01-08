#include "gtest/gtest.h"
#include "Cube.h"


TEST(CubeTest, moveCubeTest) {

    Cube c1;
    Cube c2;
    EXPECT_EQ(c1, c2) << "default cube should be equal";
    c2.rotate(white, true);
    EXPECT_NE(c1, c2) << "after one rot, should not be equal";
    c2.rotate(white, false);
    EXPECT_EQ(c1, c2) << "cube should be equal after inverse rot";
    Cube c3;
    c3.rotate(white, false);
    c3.rotate(white, false);
    c3.rotate(white, false);
    c3.rotate(white, false);
    EXPECT_EQ(c1, c3) << "cube should be equal after 4 identical rot";

}
