#include "CubeSolver.h"

#include "Cube.h"

void CubeSolver::setStartingCube(const Cube& c)
{
    m_CubeToSolve = std::make_unique<Cube>(c);
    m_StepSolution.clear();
}

bool CubeSolver::solve()
{
    m_StepSolution.clear();
    if (!m_CubeToSolve->isSolved())
    {
        computeSolution();
        return !m_StepSolution.empty();
    }

    return true;
}

bool CubeSolver::solveWhiteCross()
{
    m_StepSolution.clear();
    if (!whiteCrossSolveAvail())
    {
        return false;
    }

    if (!m_CubeToSolve->isSolved())
    {
        computeSolution();
        return !m_StepSolution.empty();
    }

    return true;
}

ColMoveSeq CubeSolver::getFullSolution() const
{
    ColMoveSeq fullSol;
    for (const ColMoveSeq& c : m_StepSolution)
    {
        fullSol.insert(fullSol.end(), c.begin(), c.end());
    }
    return fullSol;
}

const std::vector< ColMoveSeq >& CubeSolver::getFullStepsSolution() const
{
    return m_StepSolution;
}

std::size_t CubeSolver::getNbSteps() const
{
    return m_StepSolution.size();
}

ColMoveSeq CubeSolver::getStepSolution(int stepIndex) const
{
    if (stepIndex < m_StepSolution.size())
    {
        return m_StepSolution[stepIndex];
    }

    return ColMoveSeq();
}
