#include "ThistleSolver.h"
#include "GenBruteSolve.h"
#include "CubeUtils.h"


namespace
{
    ColMoveSeq solveIterative(
        const Cube& cubeToSolve,
        const GenBruteSolve::AllowedMoves& moves,
        const GenBruteSolve::SolvedCondition& cond,
        int maxDepth)
    {
        std::vector<int> depthToTry;
        depthToTry.push_back(maxDepth);
        for (int curDepth = maxDepth-1; curDepth>0; curDepth --)
        {
            depthToTry.push_back(curDepth);
        }

        GenBruteSolve solver(cond, moves);
        ColMoveSeq curSolution;
        for (auto depthIter = depthToTry.rbegin(); depthIter != depthToTry.rend() && curSolution.empty(); ++depthIter)
        {
            solver.setStartingCube(cubeToSolve);
            solver.solve(*depthIter);
            curSolution = solver.getFullSolution();
        }
        return curSolution;
    }
}
bool edgePieceIsPhaseOneGood(const EdgeCube& e)
{
    bool retVal = false;
    // if edge has white or yellow
    //  bring the piece to its w-y by rotating its non-wy pos.
    //  it is good it
    if (e.hasInColor(white) || e.hasInColor(yellow))
    {
        RubikColor whiteOrYellow = e.hasInColor(white) ? white : yellow;
        RubikColor nonWhiteYellowPos = e.getPositionNot(white, yellow);
        EdgeCube e2 = CubeUtils::turnUntillElem(e, nonWhiteYellowPos, [&whiteOrYellow](const EdgeCube& _e){return _e.hasInPosition(whiteOrYellow);});
        RubikColor nonWhiteYellowCol = e.getColorNot(whiteOrYellow);
        auto whiteYellowTurnsToHome = CubeUtils::turnUntill(e2, whiteOrYellow, [&nonWhiteYellowCol](const EdgeCube& _e){return _e.hasInPosition(nonWhiteYellowCol);});
        EdgeCube e3 = e2;
        if (whiteYellowTurnsToHome.second > 0)
            e3.rotate(whiteOrYellow, whiteYellowTurnsToHome.first, whiteYellowTurnsToHome.second);
        bool isWellOriented = e3.isSolved();
        retVal = (isWellOriented == (whiteYellowTurnsToHome.second%2 == 0));
    }
    else
    {
        EdgeCube e2 = e;
        if (e2.hasInPosition(white))
            e2.rotate(e2.getPositionNot(white), true);
        else if (e2.hasInPosition(yellow))
            e2.rotate(e2.getPositionNot(yellow), true);

        if (e2.isAtSolvedPosition())
            retVal = e2.isSolved();
        else
        {
            RubikColor pos1 = e2.getPositionNot(white); // a position
            e2.rotate(pos1, true, 2);
            if (e2.isAtSolvedPosition())
                retVal = e2.isSolved();
            else
            {
                RubikColor pos2 = e2.getPositionNot(pos1); // a position
                e2.rotate(pos2, true, 2);
                if (e2.isAtSolvedPosition())
                    retVal = e2.isSolved();
                else
                {
                    RubikColor pos3 = e2.getPositionNot(pos2); // a position
                    e2.rotate(pos3, true, 2);
                    if (e2.isAtSolvedPosition())
                        retVal = e2.isSolved();
                    else
                    {
                        RASSERT(false, "should not happen");
                    }
                }
            }
        }
    }

    RASSERT(retVal==(e.getOrientation()==WellOriented), "");
    return retVal;
}

bool phaseOneDone(const Cube& c)
{
    return std::all_of(c.getEdges().begin(), c.getEdges().end(), edgePieceIsPhaseOneGood);
};

bool edgePieceIsPhaseTwoGood(const EdgeCube& e)
{
    if ( !e.hasInColor(blue) && !e.hasInColor(green)  )
    {
        return !e.hasInPosition(blue) && !e.hasInPosition(green);
    }
    else
    {
        return true;
    }
}

bool computeCornerPieceIsPhaseTwoGood(const CornerCube& c)
{
    bool retVal =  (
        (   (c.getColor()[0] == green || c.getColor()[0] == blue) &&
            (c.getPosition()[0] == green || c.getPosition()[0] == blue)) ||
        (   (c.getColor()[1] == green || c.getColor()[1] == blue) &&
            (c.getPosition()[1] == green || c.getPosition()[1] == blue)) ||
        (   (c.getColor()[2] == green || c.getColor()[2] == blue) &&
            (c.getPosition()[2] == green || c.getPosition()[2] == blue))
    );

    return retVal;
}

bool cornerPieceIsPhaseTwoGood(const CornerCube &c)
{
    RASSERT(computeCornerPieceIsPhaseTwoGood(c) == (c.getOrientation() == WellOriented), "");
    return (c.getOrientation() == WellOriented);
}

bool phaseTwoDone(const Cube &c)
{
    return
        std::all_of(c.getCorners().begin(), c.getCorners().end(), cornerPieceIsPhaseTwoGood) &&
        std::all_of(c.getEdges().begin(), c.getEdges().end(), edgePieceIsPhaseTwoGood);
};

template <class CubeletType>
bool cubeletIsPhaseThreeGood(const CubeletType& c)
{
    for (int i = 0 ; i < c.getColor().size(); i++)
    {
        if (c.getColor()[i] != c.getPosition()[i] &&
        c.getColor()[i] != oppositeColor(c.getPosition()[i])
        )
            return false;
    }
    return true;
}

bool cornerIsPhaseThreeGood(const CornerCube& c)
{
    return cubeletIsPhaseThreeGood(c);
}
bool edgeIsPhaseThreeGood(const EdgeCube& e)
{
    return cubeletIsPhaseThreeGood(e);
}

bool phaseThreeDone(const Cube& c)
{
    return
        std::all_of(c.getCorners().begin(), c.getCorners().end(), cornerIsPhaseThreeGood) &&
        std::all_of(c.getEdges().begin(), c.getEdges().end(), edgeIsPhaseThreeGood);
};

bool ThistleSolver::isPhaseOneSolved(const Cube& c)
{
    return phaseOneDone(c);
}
bool ThistleSolver::isPhaseTwoSolved(const Cube& c)
{
    return phaseTwoDone(c);
}
bool ThistleSolver::isPhaseThreeSolved(const Cube& c)
{
    return phaseThreeDone(c);
}


ThistleSolver::ThistleSolver()
{

}

ThistleSolver::~ThistleSolver()
{

}

void ThistleSolver::computeSolution()
{}

void ThistleSolver::solvePhaseOne()
{
    std::cout<< "Solving pahse one" << std::endl;
    m_CurrentCubeState = m_CubeToSolve;

    GenBruteSolve::AllowedMoves moveAllowed({
        {{green,true},1},
        {{orange,true},1},
        {{white,true},1},
        {{yellow,true},1},
        {{red, true},1},
        {{blue, true},1}
    });

    ColMoveSeq curSolution = solveIterative(m_CurrentCubeState, moveAllowed, phaseOneDone, 7);

    m_CurrentCubeState.apply(curSolution);
    RASSERT(phaseOneDone(m_CurrentCubeState), "Patate");

    m_StepSolution.push_back(ColMoveSeq());
    for (auto m : curSolution)
        m_StepSolution.back().push_back(m);
}

void ThistleSolver::solvePhaseTwo()
{
    std::cout<< "Solving pahse two" << std::endl;
    m_CurrentCubeState = m_CubeToSolve;

    GenBruteSolve::AllowedMoves moveAllowed({
        {{green,true},1},
        {{orange,true},1},
        {{white,true},2},
        {{yellow,true},2},
        {{red, true},1},
        {{blue, true},1}
    });

    ColMoveSeq curSolution = solveIterative(m_CurrentCubeState, moveAllowed, phaseTwoDone, 7);

    m_CurrentCubeState.apply(curSolution);
    RASSERT(phaseTwoDone(m_CurrentCubeState), "Patate");

    m_StepSolution.push_back(ColMoveSeq());
    for (auto m : curSolution)
        m_StepSolution.back().push_back(m);
}

void ThistleSolver::solvePhaseThree()
{
    m_CurrentCubeState = m_CubeToSolve;

    GenBruteSolve::AllowedMoves moveAllowed({
        {{orange,true},2},
        {{red, true},2},
        {{white,true},2},
        {{yellow,true},2},
        {{green,true},1},
        {{blue, true},1}
    });

    ColMoveSeq curSolution = solveIterative(m_CurrentCubeState, moveAllowed, phaseThreeDone, 13);

    m_CurrentCubeState.apply(curSolution);
    RASSERT(phaseThreeDone(m_CurrentCubeState), "");

    m_StepSolution.push_back(ColMoveSeq());
    for (auto m : curSolution)
        m_StepSolution.back().push_back(m);
}
