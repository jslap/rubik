#include "gtest/gtest.h"
#include "Cube.h"
#include "Solver/DummyCubeSolver.h"
#include "Solver/FridrichSolver.h"
#include "Solver/ThistleSolver.h"
#include "Solver/GenBruteSolve.h"

#include "BaseSolveTests.h"

class DummySolverTest : public BaseSolverTest {
protected:

    virtual std::unique_ptr<CubeSolver> createSolver()
    {
        return std::unique_ptr<CubeSolver>(new DummyCubeSolver());
    }
};

class DummySolverStepCheckTest : public DummySolverTest 
{
protected:
    void testSolveUntilStep(const Cube& cube, int lastStep)
    {
        Cube solvedCube = cube;
        solveUntilStep(solvedCube, lastStep);

        EXPECT_TRUE(checkSolvedStep(solvedCube, lastStep));
    }

    void solveUntilStep(Cube& cube, int lastStep)
    {
        DummyCubeSolver solver;
        solver.setStartingCube(cube);

        if (lastStep >= 1)
        {
            solver.solveStepCross();
        }
        // ...

        cube.apply(solver.getFullSolution());
    }

    ::testing::AssertionResult checkSolvedStep(const Cube& cube, int lastStep)
    {
        switch (lastStep)
        {
            case 1:
                if (DummyCubeSolver::isWhiteCrossSolved(cube))
                {
                    return ::testing::AssertionSuccess();
                }
                else
                {
                    return ::testing::AssertionFailure() << cube << " is not solved for step " << lastStep;
                }
        }
        return ::testing::AssertionFailure() << "Step not well defined for solving:  " << lastStep;
    }



};
TEST_F(DummySolverStepCheckTest, dummyStepSolveCubeTest) 
{
    auto stepsToTest = {1};
    for (auto step : stepsToTest)
    {
        testSolveUntilStep(defaultCube, step);
        testSolveUntilStep(oneMoveSolve, step);
        testSolveUntilStep(c1, step);
        testSolveUntilStep(c2, step);
        testSolveUntilStep(c3, step);
    }
}

TEST_F(DummySolverTest, dummyDefaultSolveCubeTest) 
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

TEST_F(DummySolverTest, oneMoveSolveCubeTest) 
{
    doSolveTest(oneMoveSolve);
}


TEST_F(DummySolverTest, DISABLED_solveCubeTest) 
{
    doSolveTest(c1);
    doSolveTest(c2);
    doSolveTest(c3);
}
