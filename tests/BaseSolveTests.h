#include "gtest/gtest.h"
#include "Cube.h"
#include "Solver/CubeSolver.h"

class BaseSolverTest : public testing::Test {
protected:
    virtual void SetUp() 
    {
        Cube aCube{};
        mCubeToTest["default"] = aCube;
        aCube.rotate({white, true});
        mCubeToTest["oneMove"] = aCube;
        aCube.rotate({white, true});
        mCubeToTest["twoMove"] = aCube;
        oneMoveSolve = aCube;
        
        moveSeq1 = {
                {white, true},{red, true},{blue, true},
                {white, true},{red, true},{blue, true},
                {white, true},{red, true},{blue, true}};
        c1.apply(moveSeq1);
        mCubeToTest["c1"] = c1;

        for (int i = 0; i <5; i ++)
        {
            moveSeq2.push_back({white, false});
            moveSeq2.push_back({red, true});
            moveSeq2.push_back({blue, false});
        }
        c2.apply(moveSeq2);
        mCubeToTest["c2"] = c2;

        for (int i = 0; i <5; i ++)
        {
            moveSeq3.push_back({white, false});
            moveSeq3.push_back({orange, true});
            moveSeq3.push_back({blue, false});
        }
        c3.apply(moveSeq3);
        mCubeToTest["c3"] = c3;
    }

    virtual std::unique_ptr<CubeSolver> createSolver() = 0;

    void doSolveTest(const Cube& cube)
    {
        auto solver = createSolver();
        solver->setStartingCube(cube);
        bool solveResult = solver->solve();

        EXPECT_TRUE(solveResult);

        ColMoveSeq fullRes = solver->getFullSolution();

        EXPECT_NE(fullRes.size(), 0) << "Should need some move to solve.";

        EXPECT_NE(cube, defaultCube) << "cube should be solved";
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
    ColMoveSeq moveSeq1;
    Cube c1;
    ColMoveSeq moveSeq2;
    Cube c2;
    ColMoveSeq moveSeq3;
    Cube c3;

    ColMoveSeq oneMoveSolveSeq;
    Cube oneMoveSolve;

    std::map<std::string, Cube> mCubeToTest;
};
