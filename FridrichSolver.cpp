#include "FridrichSolver.h"
#include "CubeHandler.h"
#include "CubeUtils.h"

#include <iostream>
using namespace std;
using namespace CubeUtils;

namespace
{
    auto missplacedCrossFilter = [](const EdgeCube& ec){
        return ec.hasInColor(white) && !ec.isSolved();
    };

    template <class Filter>
    vector<EdgeCube> getEdgeCubiesIf(const Cube& c, Filter f)
    {
        vector<EdgeCube> crossCubies;
        std::copy_if(c.getEdges().begin(), c.getEdges().end(), back_inserter(crossCubies), f);
        return crossCubies;
    }
}

FridrichCubeSolver::FridrichCubeSolver()
{

}

FridrichCubeSolver::~FridrichCubeSolver()
{

}

void FridrichCubeSolver::_addAndApply(RubikColor col, bool direction, int nbTurn)
{
    for (int i = 0; i < nbTurn; i++)
        _addAndApply(ColMove(col, direction));
}

void FridrichCubeSolver::_addAndApply(ColMove aMove)
{
    m_StepSolution.back().push_back(aMove);
    m_CurrentCubeState.rotate(aMove);
}

void FridrichCubeSolver::_addAndApply(const ColMoveSeq& seq)
{
    for (auto m : seq)
        _addAndApply(m);
}

void FridrichCubeSolver::_addAndApply(const CubeHandler& handler, PosMove aMove)
{
    _addAndApply(handler.convertMove(aMove));
}

void FridrichCubeSolver::fastTopCross()
{
    vector<EdgeCube> crossCubies = getEdgeCubiesIf(m_CurrentCubeState, missplacedCrossFilter);
    if (crossCubies.empty())
        return;

    m_StepSolution.push_back(ColMoveSeq());
    if (crossCubies.size() == 4)
    {
        // find the best white rotation to make.
        int bestNbWhiteRot = 0;
        int bestNbWellPlaced = 0;
        const std::vector<int> NbRotToTry({1,3,2});
        //for (int nbRot: NbRotToTry)
        for (int nbRot = 1; nbRot<=3; nbRot++)
        {
            Cube tmpCube = m_CurrentCubeState;
            tmpCube.rotate(ColMove(white, true), nbRot);
            int curWellPlaced = (4 - getEdgeCubiesIf(tmpCube, missplacedCrossFilter).size());
            if (curWellPlaced > bestNbWhiteRot)
            {
                bestNbWellPlaced = curWellPlaced;
                bestNbWhiteRot = nbRot;
            }
        }

        if (bestNbWellPlaced > 0)
        {
            if (bestNbWhiteRot == 1)
                _addAndApply(ColMove(white, true));
            else if (bestNbWhiteRot == 3)
                _addAndApply(ColMove(white, false));
            else
            {
                _addAndApply(ColMove(white, true));
                _addAndApply(ColMove(white, true));
            }
        }
    }

    crossCubies = getEdgeCubiesIf(m_CurrentCubeState, missplacedCrossFilter);
    while (!crossCubies.empty())
    {
        // find one with yellow first
        EdgeCube cubeToTreat = crossCubies.front();
        if (crossCubies.size() > 1)
        {
            auto yellowMisplacedIter = std::find_if(crossCubies.begin(), crossCubies.end(), [](const EdgeCube& c) -> bool
                { return c.hasInPosition(yellow); });
            if (yellowMisplacedIter != crossCubies.end())
                cubeToTreat = *yellowMisplacedIter;
        }

        Cube cubeBefore = m_CurrentCubeState;
        // std::cout << "bring cubie to cross: " << cubeToTreat << std::endl;
        bringToCross(cubeToTreat);

        if (!m_CurrentCubeState.getEdge(cubeToTreat).isSolved())
        {
            printf("Error!\n");
            m_CurrentCubeState = cubeBefore;
            bringToCross(cubeToTreat);
            break;;
        }

        crossCubies = getEdgeCubiesIf(m_CurrentCubeState, missplacedCrossFilter);
    }


    //TODO check for super flip candidate. RFLBRD


    // TODO check for FR2D2

    if (m_StepSolution.back().empty())
        m_StepSolution.pop_back();
}

void FridrichCubeSolver::bringToCross(EdgeCube c)
{
    RubikColor otherCol = c.getColorNot(white);
    if (c.hasInPosition(white) && c.hasInPosition(otherCol))
    {

        // F R- D- R F- F-
        CubeHandler handler = CubeHandler::fromTopFront(white, otherCol);
        _addAndApply(handler, PosMove(Front, true));
        _addAndApply(handler, PosMove(Right, false));
        _addAndApply(handler, PosMove(Down, false));
        _addAndApply(handler, PosMove(Right, true));
        _addAndApply(handler, PosMove(Front, false));
        _addAndApply(handler, PosMove(Front, false));
        return;
    }

    if (c.hasInPosition(white))
    {
        // bring it to the yellow, and treat as it
        RubikColor sideToTurn = c.positionForColor(white);
        _addAndApply(ColMove(sideToTurn, true));
        _addAndApply(ColMove(sideToTurn, true));
        c = m_CurrentCubeState.getEdge(c);
    }

    if (c.hasInPosition(yellow))
    {
        // bring it on the right other face.
        auto bringTorightSide = turnUntill(m_CurrentCubeState, yellow, [&c, otherCol](const Cube& cu){return cu.getEdge(c).hasInPosition(otherCol);});
        _addAndApply(yellow, bringTorightSide.first, bringTorightSide.second);

        EdgeCube curEdgeCube = m_CurrentCubeState.getEdge(c);
        if (curEdgeCube.positionForColor(otherCol) == otherCol) // simple rotation would do.
        {
            _addAndApply(ColMove(otherCol, true));
            _addAndApply(ColMove(otherCol, true));
        }
        else
        {
            // Handle from top is white and front is otherCol
            // F- R- D- R F- F-
            CubeHandler handler = CubeHandler::fromTopFront(white, otherCol);
            _addAndApply(handler, PosMove(Front, false));
            _addAndApply(handler, PosMove(Right, false));
            _addAndApply(handler, PosMove(Down, false));
            _addAndApply(handler, PosMove(Right, true));
            _addAndApply(handler, PosMove(Front, false));
            _addAndApply(handler, PosMove(Front, false));
        }
    }
    else // it is niether on white or yellow.
    {
        //First, check if it is on the right side
        if (c.positionForColor(otherCol) == otherCol)
        {
            // just turn the current side until it is right
            auto bringToSolve = turnUntill(m_CurrentCubeState, otherCol, [&c](const Cube& cu){return cu.getEdge(c).isSolved();});
            _addAndApply(otherCol, bringToSolve.first, bringToSolve.second);
        }
        else if (c.hasInPosition(otherCol))
        {
            EdgeCube cCopy = c;
            cCopy.rotate(otherCol, true);
            bool sideOfCubieOnRight = (cCopy.getPositionNot(otherCol)==yellow);
            if (sideOfCubieOnRight) //is the cubie on the right of the otherCol face?
            {
                // R- D- R F- F-
                CubeHandler handler = CubeHandler::fromTopFront(white, otherCol);
                _addAndApply(handler, PosMove(Right, false));
                _addAndApply(handler, PosMove(Down, false));
                _addAndApply(handler, PosMove(Right, true));
                _addAndApply(handler, PosMove(Front, false));
                _addAndApply(handler, PosMove(Front, false));
            }
            else
            {
                // Revrese: R- D- R F- F-
                CubeHandler handler = CubeHandler::fromTopFront(white, otherCol);
                _addAndApply(handler, PosMove(Left, true));
                _addAndApply(handler, PosMove(Down, true));
                _addAndApply(handler, PosMove(Left, false));
                _addAndApply(handler, PosMove(Front, false));
                _addAndApply(handler, PosMove(Front, false));
            }
        }
        else
        {
            //work on the side that do not have the whte color on... it is faster.
            RubikColor otherSide = c.positionForColor(otherCol);
            // Bring it on the yellow.
            if (!m_CurrentCubeState.findCubieByColor(EdgeCoord({{otherSide, white}})).isSolved()) //if its white side is not placed yet...
            {
                // just turn it to yellow.
                auto bringToYellowSide = turnUntill(m_CurrentCubeState, otherSide, [&c, otherCol](const Cube& cu){return cu.getEdge(c).hasInPosition(yellow);});
                _addAndApply(otherSide, bringToYellowSide.first, bringToYellowSide.second);
            }
            else
            {
                RubikColor whiteSide = c.positionForColor(white);
                // cout << "before: " << m_CurrentCubeState.getEdge(c) << std::endl;
                EdgeCube cCopy = c;
                cCopy.rotate(whiteSide, true);
                bool sideOfCubieOnRight = (cCopy.getPositionNot(whiteSide)==yellow);
                //R- D- R
                CubeHandler handler = CubeHandler::fromTopFront(white, whiteSide);
                if (sideOfCubieOnRight)
                {
                    _addAndApply(handler, PosMove(Right, false));
                    // cout << "case 1 - 1: " << m_CurrentCubeState.getEdge(c) << std::endl;
                    _addAndApply(handler, PosMove(Down, false));
                    // cout << "case 1 - 2: " << m_CurrentCubeState.getEdge(c) << std::endl;
                    _addAndApply(handler, PosMove(Right, true));
                }
                else
                {
                    _addAndApply(handler, PosMove(Left, true));
                    // cout << "case 2 - 1: " << m_CurrentCubeState.getEdge(c) << std::endl;
                    _addAndApply(handler, PosMove(Down, false));
                    // cout << "case 2 - 2: " << m_CurrentCubeState.getEdge(c) << std::endl;
                    _addAndApply(handler, PosMove(Left, false));
                }
            }
            // cout << "on yellow: " << m_CurrentCubeState.getEdge(c) << std::endl;

            // bring it on the right side by turning the yellow.
            auto bringToRightSide = turnUntill(m_CurrentCubeState, yellow, [&c, otherCol](const Cube& cu){return cu.getEdge(c).hasInPosition(otherCol);});
            _addAndApply(yellow, bringToRightSide.first, bringToRightSide.second);
            // cout << "right yellow: " << m_CurrentCubeState.getEdge(c) << std::endl;

            // And then, double turn to pu it on the cross.
            _addAndApply(ColMove(otherCol, true));
            _addAndApply(ColMove(otherCol, true));
            // cout << "solved: " << m_CurrentCubeState.getEdge(c) << std::endl;
        }
    }

}


void FridrichCubeSolver::computeSolution()
{
    m_CurrentCubeState = m_CubeToSolve;
    fastTopCross();
}

void FridrichCubeSolver::computeWhiteCross()
{
    m_CurrentCubeState = m_CubeToSolve;
    fastTopCross();
}
