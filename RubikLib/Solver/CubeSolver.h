#pragma once

#include "RubikBase.h"

class Cube;
class CubeSolver
{
public:
    CubeSolver() = default;;
    virtual ~CubeSolver() = default;

    void setStartingCube(const Cube& c);

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

    std::unique_ptr<Cube> m_CubeToSolve;
    std::vector< ColMoveSeq > m_StepSolution;
};
