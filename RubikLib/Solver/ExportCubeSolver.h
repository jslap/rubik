#pragma once

#include "CubeSolver.h"
#include "ExportSolverConvert.h"
#include "RubikBase.h"

class ExportCubeSolver : public CubeSolver
{
public:
    ExportCubeSolver() = default;;
    virtual ~ExportCubeSolver() = default;

protected:
    // compute solution, do not alter the cube to solve.
    virtual void computeSolution();
    virtual bool whiteCrossSolveAvail() const {return false;};
    virtual void computeWhiteCross() {};

private:
    int phase;
    ColMoveSeq dosolve( const ExportSolverConvert::CubeVecInt startState ) ;
    ExportSolverConvert::CubeVecInt computeId ( ExportSolverConvert::CubeVecInt state ) ;
};
