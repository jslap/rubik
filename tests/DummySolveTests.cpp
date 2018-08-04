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


TEST_F(DummySolverTest, DISABLED_dummyDefaultSolveCubeTest) 
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

TEST_F(DummySolverTest, DISABLED_oneMoveSolveCubeTest) 
{
    doSolveTest(oneMoveSolve);
}


TEST_F(DummySolverTest, DISABLED_solveCubeTest) 
{
    doSolveTest(c1);
    doSolveTest(c2);
    doSolveTest(c3);
}
