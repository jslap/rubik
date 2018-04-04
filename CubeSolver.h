#pragma once

#include "Cube.h"

class CubeSolver
{
public:
    CubeSolver():m_CubeToSolve() {};
    virtual ~CubeSolver() {};

    void setStartingCube(const Cube& c)
    {
        m_CubeToSolve = c;
        m_StepSolution.clear();
    }

    // clear the results and compute.
    bool solve();
    bool solveWhiteCross();


    //Getting the results
    ColMoveSeq getFullSolution() const;
    int getNbSteps() const;
    ColMoveSeq getStepSolution(int stepIndex) const;

protected:
    // compute solution, do not alter the cube to solve.
    virtual void computeSolution() = 0;
    virtual bool whiteCrossSolveAvail() const {return false;};
    virtual void computeWhiteCross() {};

    Cube m_CubeToSolve;
    std::vector< ColMoveSeq > m_StepSolution;
};
