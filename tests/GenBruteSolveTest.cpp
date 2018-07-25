
TEST(GenBruteSolveTest, solveACube)
{
    const Cube defaultCube;

    Cube mixedCube;
    mixedCube.rotate(white, true);
    mixedCube.rotate(red, false);
    mixedCube.rotate(blue, false);
    mixedCube.rotate(white, false);
}



TEST(GenBruteSolveTest, testPhaseOne)
{
    const Cube defaultCube;
    EXPECT_TRUE(ThistleSolver::isPhaseOneSolved(defaultCube));

    Cube mixedCube;
    Cube mixedPhaseOneOkCube;
    Cube mixedPhaseOneNotOkCube;
    for (int i =0; i < 13; i++)
    {
        mixedCube.rotate(white, (i%3 == 0));
        mixedCube.rotate(red, (i%3 == 0));
        mixedCube.rotate(blue, (i%3 == 0));
        mixedCube.rotate(white, (i%3 == 0));
        mixedCube.rotate(red, (i%3 == 0));
        mixedCube.rotate(blue, (i%3 == 0));
        mixedCube.rotate(white, (i%3 == 0));
        mixedCube.rotate(red, (i%3 == 0));
        mixedCube.rotate(blue, (i%3 == 0));

        mixedPhaseOneOkCube.rotate(red, (i%3 == 0));
        mixedPhaseOneOkCube.rotate(blue, (i%3 == 0));
        mixedPhaseOneOkCube.rotate(orange, (i%3 == 0));
        mixedPhaseOneOkCube.rotate(blue, (i%3 == 0));
        mixedPhaseOneOkCube.rotate(green, (i%3 == 0));
        mixedPhaseOneOkCube.rotate(blue, (i%3 == 0));

        if (i == 6)
        {
            mixedPhaseOneNotOkCube.rotate(white, true);
        }
        mixedPhaseOneNotOkCube.rotate(red, (i%3 == 0));
        mixedPhaseOneNotOkCube.rotate(blue, (i%3 == 0));
        mixedPhaseOneNotOkCube.rotate(orange, (i%3 == 0));
        mixedPhaseOneNotOkCube.rotate(blue, (i%3 == 0));
        mixedPhaseOneNotOkCube.rotate(green, (i%3 == 0));
        mixedPhaseOneNotOkCube.rotate(blue, (i%3 == 0));
    }

    return;
    EXPECT_TRUE(ThistleSolver::isPhaseOneSolved(mixedPhaseOneOkCube));
    EXPECT_FALSE(ThistleSolver::isPhaseOneSolved(mixedPhaseOneNotOkCube));
    ThistleSolver solver;

    solver.setStartingCube(mixedCube);
    solver.solvePhaseOne();

    Cube solvedPhaseOneCube = mixedCube;
    EXPECT_FALSE(ThistleSolver::isPhaseTwoSolved(mixedCube));
    EXPECT_FALSE(ThistleSolver::isPhaseOneSolved(mixedCube));
    solvedPhaseOneCube.apply(solver.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseOneSolved(solvedPhaseOneCube));

    //Phase two
    ThistleSolver solver2;
    EXPECT_FALSE(ThistleSolver::isPhaseTwoSolved(solvedPhaseOneCube));
    solver2.setStartingCube(solvedPhaseOneCube);
    solver2.solvePhaseTwo();

    Cube solvedPhaseTwoCube = solvedPhaseOneCube;
    EXPECT_FALSE(ThistleSolver::isPhaseTwoSolved(solvedPhaseOneCube));
    solvedPhaseTwoCube.apply(solver2.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseTwoSolved(solvedPhaseTwoCube));

    return; // skip phase three...
    //Phase three
    ThistleSolver solver3;
    EXPECT_FALSE(ThistleSolver::isPhaseThreeSolved(solvedPhaseTwoCube));
    solver2.setStartingCube(solvedPhaseTwoCube);
    solver2.solvePhaseThree();

    Cube solvedPhaseThreeCube = solvedPhaseTwoCube;
    EXPECT_FALSE(ThistleSolver::isPhaseThreeSolved(solvedPhaseTwoCube));
    solvedPhaseThreeCube.apply(solver2.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseThreeSolved(solvedPhaseThreeCube));

}

TEST(DISABLED_GenBruteSolveTest, testSomePhases)
{
    const Cube defaultCube;

    Cube mixedCube;
    // U1-D2-R3-B3-D2-F3
    // up- white
    // down - yellow
    // front - red
    // left  - green
    // right - blue
    // back - orange
    for (int i =0; i < 23; i++)
    {
        mixedCube.rotate(white, true, 1);
        mixedCube.rotate(yellow, true, 2);
        mixedCube.rotate(blue, false, 1);
        mixedCube.rotate(orange, false, 1);
        mixedCube.rotate(yellow, true, 2);
        mixedCube.rotate(red, false, 1);
    }

    // mixedCube.rotate(red, false, 1);
    // mixedCube.rotate(red, true, 1);
    // mixedCube.rotate(red, false, 1);
    // mixedCube.rotate(red, false, 1);

    ThistleSolver solver;

    solver.setStartingCube(mixedCube);
    solver.solvePhaseOne();

    Cube solvedPhaseOneCube = mixedCube;
    EXPECT_FALSE(ThistleSolver::isPhaseOneSolved(mixedCube));
    solvedPhaseOneCube.apply(solver.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseOneSolved(solvedPhaseOneCube));

    //Phase two
    ThistleSolver solver2;
    EXPECT_FALSE(ThistleSolver::isPhaseTwoSolved(solvedPhaseOneCube));
    solver2.setStartingCube(solvedPhaseOneCube);
    solver2.solvePhaseTwo();

    Cube solvedPhaseTwoCube = solvedPhaseOneCube;
    EXPECT_FALSE(ThistleSolver::isPhaseTwoSolved(solvedPhaseOneCube));
    solvedPhaseTwoCube.apply(solver2.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseTwoSolved(solvedPhaseTwoCube));
    return;

    //Phase three
    ThistleSolver solver3;
    EXPECT_FALSE(ThistleSolver::isPhaseThreeSolved(solvedPhaseTwoCube));
    solver2.setStartingCube(solvedPhaseTwoCube);
    solver2.solvePhaseThree();

    Cube solvedPhaseThreeCube = solvedPhaseTwoCube;
    EXPECT_FALSE(ThistleSolver::isPhaseThreeSolved(solvedPhaseTwoCube));
    solvedPhaseThreeCube.apply(solver2.getFullSolution());
    EXPECT_TRUE(ThistleSolver::isPhaseThreeSolved(solvedPhaseThreeCube));

}
