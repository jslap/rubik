#include "gtest/gtest.h"
#include "Cube.h"
#include "Solver/ExportCubeSolver.h"

#include "BaseSolveTests.h"

class ExportSolverTest : public BaseSolverTest {
protected:

    virtual std::unique_ptr<CubeSolver> createSolver()
    {
        return std::unique_ptr<CubeSolver>(new ExportCubeSolver());
    }

    void testExport(const Cube& c) 
    {
        auto convC1 = ExportSolverConvert::cubeToExport(c1);
        auto convConvC1 = ExportSolverConvert::cubeFromExport(convC1);
        EXPECT_TRUE(convConvC1.isValidCube()) << "Conversion is not valid: " << c;
        EXPECT_EQ(convConvC1.getEdges(), c1.getEdges()) << "Conversion edges are not good";
        EXPECT_EQ(convConvC1.getCorners(), c1.getCorners()) << "Conversion corners are not good";
        EXPECT_EQ(convConvC1, c1) << "Conversion is not good";
    }

    int convertMove(const ColMove& move)
    {
        int retVal = 0;
        switch (move.first)
        {
            case green: 
                retVal = 0;
                break;
            case blue: 
                retVal = 1;
                break;
            case yellow: 
                retVal = 2;
                break;
            case white: 
                retVal = 3;
                break;
            case red: 
                retVal = 4;
                break;
            case orange: 
                retVal = 5;
                break;
            default:
                retVal = -1;
                throw(std::invalid_argument(""));
        }
        retVal *= 3;

        if (!move.second)
        {
            retVal += 2;
        }

        return retVal;
    }
    std::vector<int> convertMoves(const ColMoveSeq& moveSeq)
    {
        std::vector<int> retVal;
        for (auto& move : moveSeq)
        {
            retVal.push_back(convertMove(move));
        }
        return retVal;
    }

    void checkExportMoves(const ColMoveSeq& moveSeq)
    {
        Cube defCube;
        ExportSolverConvert::CubeVecInt convDefCube = ExportSolverConvert::cubeToExport(defCube);

        defCube.apply(moveSeq);
        ExportSolverConvert::CubeVecInt convRotatedCube = ExportSolverConvert::cubeToExport(defCube);

        auto convertedMoves = convertMoves(moveSeq);
        ExportSolverConvert::CubeVecInt rotatedCubeConv = convDefCube;
        for (int m : convertedMoves)
        {
            rotatedCubeConv = ExportSolverConvert::applyMove(m, rotatedCubeConv);
        }

        EXPECT_EQ(convRotatedCube, rotatedCubeConv) << "Fail to export " << moveSeq;
    }
};


TEST_F(ExportSolverTest, testConvert)
{
    for (auto c : RubikBase::RubikColors())
    {
        checkExportMoves({{c, true}});
        checkExportMoves({{c, false}});
        checkExportMoves({{c, false},{c, false}});
    }
    checkExportMoves(moveSeq1);
    checkExportMoves(moveSeq2);
    checkExportMoves(moveSeq3);
}

TEST_F(ExportSolverTest, dummyDefaultSolveCubeTest) 
{
    ExportCubeSolver solver;
    solver.setStartingCube(defaultCube);
    bool solveResult = solver.solve();

    EXPECT_TRUE(solveResult);

    ColMoveSeq fullRes = solver.getFullSolution();

    EXPECT_EQ(fullRes.size(), 0) << "Should need no move to solve.";
}

TEST_F(ExportSolverTest, oneMoveSolveCubeTest) 
{
    doSolveTest(oneMoveSolve);
}

TEST_F(ExportSolverTest, solveCubeTest) 
{
    doSolveTest(c1);
    doSolveTest(c2);
    doSolveTest(c3);
}

TEST_F(ExportSolverTest, cubeConvertTest)
{
    for (const auto& testCube : mCubeToTest)
    {
        SCOPED_TRACE("Exporting " + testCube.first);

        testExport(testCube.second);
    }
}
