
::testing::AssertionResult isCubeFridrichTopCrossSolvable(Cube mixedCube) 
{
    FridrichCubeSolver solver;
    solver.setStartingCube(mixedCube);
    bool solveResult = solver.solveWhiteCross();
    if (!solveResult)
    {
        return ::testing::AssertionFailure() << "Solve returned False";
    }

    Cube solvedCube = mixedCube;
    EXPECT_FALSE(mixedCube.isCrossSolved(white));
    if (mixedCube.isCrossSolved(white))
    {
        return ::testing::AssertionFailure() << "Cube was solved before solving";
    }
    solvedCube.apply(solver.getFullSolution());
    EXPECT_TRUE(solvedCube.isCrossSolved(white));
    if (!mixedCube.isCrossSolved(white))
    {
        return ::testing::AssertionFailure() << "Cube has not been solve";
    }

    return ::testing::AssertionSuccess();
}

TEST(FridrichSolveTest, solveOne)
{
    Cube mixedCube1;
    mixedCube1.rotate(white, true);
    mixedCube1.rotate(red, true);
    mixedCube1.rotate(blue, true);
    mixedCube1.rotate(white, true);
    mixedCube1.rotate(red, true);
    mixedCube1.rotate(blue, true);
    mixedCube1.rotate(white, true);
    mixedCube1.rotate(red, true);
    mixedCube1.rotate(blue, true);
    // isCubeFridrichTopCrossSolvable(mixedCube1);

    Cube mixedCube2;
    mixedCube2.rotate(red, true);
    // isCubeFridrichTopCrossSolvable(mixedCube2);
}
