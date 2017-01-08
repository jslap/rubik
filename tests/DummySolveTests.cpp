#include "gtest/gtest.h"
#include "Cube.h"
#include "DummyCubeSolver.h"


TEST(DummySolverTest, solveCubeTest) {
    const Cube defaultCube;

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

    DummyCubeSolver solver;
    solver.setStartingCube(mixedCube);
    bool solveResult = solver.solve();

    EXPECT_TRUE(solveResult);

    ColMoveSeq fullRes = solver.getFullSolution();
    EXPECT_NE(fullRes.size(), 0) << "Should need some move to solve.";

    Cube solvedCube = mixedCube;
    solvedCube.apply(fullRes);
    EXPECT_EQ(solvedCube, defaultCube) << "cube should be solved";

    ColMoveSeq alteredResult = fullRes;
    alteredResult.erase(std::next(alteredResult.begin(), alteredResult.size()/2));
    Cube unsolvedCube = mixedCube;
    unsolvedCube.apply(alteredResult);
    EXPECT_NE(unsolvedCube, defaultCube) << "cube should not be solved";

}
