#include "CubeSolver.h"


bool CubeSolver::solve()
{
    if (!m_CubeToSolve.isSolved())
    {
        computeSolution();
        return m_StepSolution.size() > 0;
    }
    else
    {
        m_StepSolution.clear();
        return true;
    }
}

ColMoveSeq CubeSolver::getFullSolution() const
{
    ColMoveSeq fullSol;
    for (const ColMoveSeq& c : m_StepSolution)
        fullSol.insert(fullSol.end(), c.begin(), c.end());
    return fullSol;
}

int CubeSolver::getNbSteps() const
{
    return m_StepSolution.size();
}

ColMoveSeq CubeSolver::getStepSolution(int stepIndex) const
{
    if (stepIndex < m_StepSolution.size())
        return m_StepSolution[stepIndex];
    else
        return ColMoveSeq();
}
