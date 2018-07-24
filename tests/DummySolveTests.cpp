#include "gtest/gtest.h"
#include "Cube.h"
#include "Solver/DummyCubeSolver.h"
#include "Solver/FridrichSolver.h"
#include "Solver/ThistleSolver.h"
#include "Solver/GenBruteSolve.h"

TEST(GenBruteSolveTest, solveACube)
{
    const Cube defaultCube;

    Cube mixedCube;
    mixedCube.rotate(white, true);
    mixedCube.rotate(red, false);
    mixedCube.rotate(blue, false);
    mixedCube.rotate(white, false);
}


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

class BaseSolverTest : public testing::Test {
protected:
    virtual void SetUp() 
    {
        c1.rotate(white, true);
        c1.rotate(red, true);
        c1.rotate(blue, true);
        c1.rotate(white, true);
        c1.rotate(red, true);
        c1.rotate(blue, true);
        c1.rotate(white, true);
        c1.rotate(red, true);
        c1.rotate(blue, true);

        for (int i = 0; i <5; i ++)
        {
            c2.rotate(white, false);
            c2.rotate(red, true);
            c2.rotate(blue, false);
        }
        for (int i = 0; i <5; i ++)
        {
            c3.rotate(white, false);
            c3.rotate(orange, true);
            c3.rotate(blue, false);
        }

    }

    void doSolveTest(const Cube& cube)
    {
        DummyCubeSolver solver;
        solver.setStartingCube(cube);
        bool solveResult = solver.solve();

        EXPECT_TRUE(solveResult);

        ColMoveSeq fullRes = solver.getFullSolution();

        // for (int i = 0; i< solver.getNbSteps(); i++)
        //     printf("step: %lu\n", solver.getStepSolution(i).size());
        EXPECT_NE(fullRes.size(), 0) << "Should need some move to solve.";

        Cube solvedCube = cube;
        solvedCube.apply(fullRes);
        EXPECT_EQ(solvedCube, defaultCube) << "cube should be solved";

        ColMoveSeq alteredResult = fullRes;
        alteredResult.erase(std::next(alteredResult.begin(), alteredResult.size()/2));
        Cube unsolvedCube = cube;
        unsolvedCube.apply(alteredResult);
        EXPECT_NE(unsolvedCube, defaultCube) << "cube should not be solved";
    }

    const Cube defaultCube;
    Cube c1;
    Cube c2;
    Cube c3;
};


TEST_F(BaseSolverTest, dummyDefaultSolveCubeTest) 
{
    DummyCubeSolver solver;
    solver.setStartingCube(defaultCube);
    bool solveResult = solver.solve();

    EXPECT_TRUE(solveResult);

    ColMoveSeq fullRes = solver.getFullSolution();

    // for (int i = 0; i< solver.getNbSteps(); i++)
    //     printf("step: %lu\n", solver.getStepSolution(i).size());
    EXPECT_EQ(fullRes.size(), 0) << "Should need no move to solve.";
}

TEST_F(BaseSolverTest, solveCubeTest) 
{
    doSolveTest(c1);
    doSolveTest(c2);
    doSolveTest(c3);
}
