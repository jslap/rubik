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
TEST(CubeTest, solveCubeTest) {
    const Cube solvedCube;

    Cube mixedCube;
    mixedCube.rotate(white, true);
    mixedCube.rotate(red, true);
    mixedCube.rotate(blue, true);
    mixedCube.rotate(white, true);
    mixedCube.rotate(red, true);
    mixedCube.rotate(blue, true);
    mixedCube.rotate(white, true);
    mixedCube.rotate(red, true);
    mixedCube.rotate(blue, true);

    Cube cubeToSolve = mixedCube;

    ColMoveSeq fullRes;
    for (int i = 0; i < 10; i++)
    {
        auto res = cubeToSolve.solve(10);
        fullRes.insert(fullRes.end(), res.begin(), res.end());
    }
    EXPECT_NE(fullRes.size(), 0) << "Should need some move to solve.";

    EXPECT_EQ(cubeToSolve, solvedCube) << "cube should be solved";

    Cube cubeToSolveIter = mixedCube;
    for (auto m : fullRes)
        cubeToSolveIter.rotate(m);

    EXPECT_EQ(cubeToSolveIter, solvedCube) << "cube should be solved";
    EXPECT_NE(mixedCube, solvedCube) << "after one rot, should not be equal";

}
