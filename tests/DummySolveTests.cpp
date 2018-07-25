#include "gtest/gtest.h"
#include "Cube.h"
#include "Solver/DummyCubeSolver.h"
#include "Solver/FridrichSolver.h"
#include "Solver/ThistleSolver.h"
#include "Solver/GenBruteSolve.h"

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
