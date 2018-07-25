
TEST(DISABLED_ThistleSolveTest, testPhaseTwo)
{
    const Cube defaultCube;
    EXPECT_TRUE(ThistleSolver::isPhaseTwoSolved(defaultCube));

    Cube mixedCube;
    for (int i =0; i < 13; i++)
    {
        mixedCube.rotate(white, true, 2);
        mixedCube.rotate(red, (i%3 == 0));
        mixedCube.rotate(blue, (i%3 == 0));
        mixedCube.rotate(white, true, 2);
        mixedCube.rotate(red, (i%3 == 0));
        mixedCube.rotate(blue, (i%3 == 0));
        mixedCube.rotate(yellow, true, 2);
        mixedCube.rotate(red, (i%3 == 0));
        mixedCube.rotate(blue, (i%3 == 0));
    }

    EXPECT_TRUE(ThistleSolver::isPhaseOneSolved(mixedCube));

    ThistleSolver solver;

    solver.setStartingCube(mixedCube);
    solver.solvePhaseTwo();

    Cube solvedPhaseOneCube = mixedCube;
    EXPECT_FALSE(ThistleSolver::isPhaseTwoSolved(mixedCube));
    solvedPhaseOneCube.apply(solver.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseTwoSolved(solvedPhaseOneCube));
}

TEST(DISABLED_ThistleSolveTest, testPhaseThree)
{
    const Cube defaultCube;
    EXPECT_TRUE(ThistleSolver::isPhaseThreeSolved(defaultCube));

    Cube mixedCube;
    for (int i =0; i < 13; i++)
    {
        mixedCube.rotate(white, true, 2);
        mixedCube.rotate(red, (i%3 == 0), 2);
        mixedCube.rotate(blue, (i%3 == 0));
        mixedCube.rotate(white, true, 2);
        mixedCube.rotate(red, (i%3 == 0), 2);
        mixedCube.rotate(blue, (i%3 == 0));
        mixedCube.rotate(yellow, true, 2);
        mixedCube.rotate(orange, (i%3 == 0), 2);
        mixedCube.rotate(blue, (i%3 == 0));
    }

    EXPECT_TRUE(ThistleSolver::isPhaseTwoSolved(mixedCube));

    ThistleSolver solver;

    solver.setStartingCube(mixedCube);
    solver.solvePhaseThree();

    Cube solvedPhaseOneCube = mixedCube;
    EXPECT_FALSE(ThistleSolver::isPhaseThreeSolved(mixedCube));
    solvedPhaseOneCube.apply(solver.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseThreeSolved(solvedPhaseOneCube));
}