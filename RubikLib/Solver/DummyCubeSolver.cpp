#include "DummyCubeSolver.h"
#include "CubeHandler.h"

DummyCubeSolver::DummyCubeSolver() = default;

DummyCubeSolver::~DummyCubeSolver() = default;

void DummyCubeSolver::computeSolution()
{
    m_CurrentCubeState = *m_CubeToSolve;

    ColMoveSeq stepSol;
    stepSol = _solveStepCross();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }

    stepSol = _solveStepWhiteLayer();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }

    stepSol = _solveStepMiddleLayer();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }

    stepSol = _solveStepTopCross();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }

    stepSol = _solveStepTopCorners();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }

    stepSol = _solveStepTopCornersPos();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }

    stepSol = _solveStepTopEdges();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }
}

void DummyCubeSolver::computeWhiteCross()
{
    m_CurrentCubeState = *m_CubeToSolve;
    ColMoveSeq stepSol;
    stepSol = _solveStepCross();
    if (!stepSol.empty())
    {
        m_StepSolution.push_back(stepSol);
    }
}


void DummyCubeSolver::_addAndApply(ColMove aMove, ColMoveSeq &seq)
{
    seq.push_back(aMove);
    m_CurrentCubeState.rotate(aMove);
}

void DummyCubeSolver::_addAndApply(const CubeHandler& handler, PosMove aMove, ColMoveSeq &seq)
{
    _addAndApply(handler.convertMove(aMove), seq);
}


ColMoveSeq DummyCubeSolver::_solveStepCrossElt(RubikColor col)
{
    const Cube defCube;
    ColMoveSeq retVal;

    RubikColor otherCol = col;
    EdgeCube fisrtElt = defCube.findCubieByColor(EdgeCoord({{white, otherCol}}));
    EdgeCube curCubie = m_CurrentCubeState.findCubieByColor(fisrtElt.getColor());
    if (curCubie != fisrtElt)
    {
        if (curCubie.hasInPosition(white) && curCubie.hasInPosition(otherCol))
        {
            //do nothing, we are ok.
        }
        else
        {
            // put the cube in the yellow face.
            if (!curCubie.hasInPosition(yellow))
            {
                if (curCubie.hasInPosition(white))
                {
                    RubikColor colToRot = curCubie.getPositionNot(white);
                    _addAndApply(ColMove(colToRot, true), retVal);
                    _addAndApply(ColMove(colToRot, true), retVal);
                }
                else
                {
                    Cube cubeCopy = m_CurrentCubeState;
                    RubikColor otherFacePos = curCubie.getPosition()[0];

                    cubeCopy.rotate(otherFacePos, true);
                    bool clockwiseOk = cubeCopy.findCubieByColor(fisrtElt.getColor()).hasInPosition(yellow);


                    _addAndApply(ColMove(otherFacePos, clockwiseOk), retVal);
                    _addAndApply(ColMove(yellow, true), retVal);
                    _addAndApply(ColMove(otherFacePos, !clockwiseOk), retVal);

                }
            }

            curCubie = m_CurrentCubeState.findCubieByColor(fisrtElt.getColor());
            RASSERT(m_CurrentCubeState.findCubieByColor(fisrtElt.getColor()).hasInPosition(yellow), "");

            // turn yellow face untill it is on the right side.
            while (!curCubie.hasInPosition(otherCol))
            {
                _addAndApply(ColMove(yellow, true), retVal);

                curCubie = m_CurrentCubeState.findCubieByColor(fisrtElt.getColor());
            }

            curCubie = m_CurrentCubeState.findCubieByColor(fisrtElt.getColor());
            RASSERT(m_CurrentCubeState.findCubieByColor(fisrtElt.getColor()).hasInPosition(otherCol) &&
                    m_CurrentCubeState.findCubieByColor(fisrtElt.getColor()).hasInPosition(yellow),
                    "");

            // make it on the white side;
            _addAndApply(ColMove(otherCol, true), retVal);
            _addAndApply(ColMove(otherCol, true), retVal);
        }

        curCubie = m_CurrentCubeState.findCubieByColor(fisrtElt.getColor());
        RASSERT(m_CurrentCubeState.findCubieByColor(fisrtElt.getColor()).hasInPosition(otherCol) &&
            m_CurrentCubeState.findCubieByColor(fisrtElt.getColor()).hasInPosition(white),"");

        if (curCubie != fisrtElt)
        {
            CubeHandler handler = CubeHandler::fromTopRight(white, otherCol);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Front, false), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
        }

        curCubie = m_CurrentCubeState.findCubieByColor(fisrtElt.getColor());
        RASSERT(curCubie == fisrtElt,"");
    }

    return retVal;

}


ColMoveSeq DummyCubeSolver::_solveStepCross()
{
    ColMoveSeq retVal;

    std::vector< RubikColor > colorVec = {blue, orange, green, red};

    for (RubikColor col : colorVec)
    {
        ColMoveSeq tmpMoveSeq = _solveStepCrossElt(col);
        retVal.insert(retVal.end(), tmpMoveSeq.begin(), tmpMoveSeq.end());
    }

    return retVal;

}

ColMoveSeq DummyCubeSolver::_solveStepWhiteLayer()
{
    ColMoveSeq retVal;

    Cube defCube;
    std::vector<CornerCube> whiteLayerCorners;
    std::copy_if(
        defCube.getCorners().begin(), defCube.getCorners().end(), 
        std::back_inserter(whiteLayerCorners),
        [] (const auto &c) {return c.hasInColor(white); });

    for (auto whiteCorner : whiteLayerCorners)
    {
        ColMoveSeq tmpMoveSeq = _solveStepWhiteLayerElt(whiteCorner.getColor());
        retVal.insert(retVal.end(), tmpMoveSeq.begin(), tmpMoveSeq.end());
    }
    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepWhiteLayerElt(const CornerCoord& piece)
{
    ColMoveSeq retVal;

    // get the other colors of the piece: other than white.
    std::vector< RubikColor > otherColVec;
    std::copy_if(
        piece.begin(), piece.end(), 
        std::back_inserter(otherColVec),
        [] (const auto &col) {return col != white; });

    RASSERT(otherColVec.size() == 2, "");

    CornerCube wantedCubie(piece);

    CornerCube curCubie = m_CurrentCubeState.findCubieByColor(piece);

    //Check that it is not at the right pos already...
    while (curCubie != wantedCubie)
    {
        // if the cube is already at the top layer, put it down
        if (curCubie.hasInPosition(white))
        {
            CubeHandler handler = CubeHandler::genHandler(curCubie, Up, Front, Right);

            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Down, false), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);

            curCubie = m_CurrentCubeState.findCubieByColor(piece);
        }
        RASSERT(curCubie.hasInPosition(yellow), "");

        // allign the cube, in the yellow layer so it is just below the right pos.
        while ( ! (curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]) ) )
        {
            _addAndApply(ColMove(yellow, true), retVal);
            curCubie = m_CurrentCubeState.findCubieByColor(piece);
        }

        RASSERT(curCubie.hasInPosition(yellow) && curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]), "");

        {

            // do these steps to try to make it right.
            CubeHandler handler = CubeHandler::genHandler(curCubie, Down, Front, Right);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Down, false), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);
            _addAndApply(handler, PosMove(Down, true), retVal);

            curCubie = m_CurrentCubeState.findCubieByColor(piece);
        }

        RASSERT(curCubie.hasInPosition(white) && curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]), "");

    }
    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepMiddleLayer()
{
    ColMoveSeq retVal;

    Cube defCube;
    std::vector<EdgeCube> middleLayerEdges;
    std::copy_if(
        defCube.getEdges().begin(), defCube.getEdges().end(), 
        std::back_inserter(middleLayerEdges),
        [] (const auto &c) {return (!c.hasInColor(white) && !c.hasInColor(yellow)); });

    for (auto &curEdge : middleLayerEdges)
    {
        ColMoveSeq tmpMoveSeq = _solveStepMiddleLayerElt(curEdge.getColor());
        retVal.insert(retVal.end(), tmpMoveSeq.begin(), tmpMoveSeq.end());
    }
    return retVal;

}

ColMoveSeq DummyCubeSolver::_solveStepMiddleLayerElt(const EdgeCoord& piece)
{
    //printf("Solve middle layer (%s,%s)\n", colorName(piece[0]).c_str(), colorName(piece[1]).c_str());
    ColMoveSeq retVal;


    EdgeCube wantedCubie(piece);

    EdgeCube curCubie = m_CurrentCubeState.findCubieByColor(piece);

    if (wantedCubie != curCubie)
    {
        // step one, make sure it is on the bottom layer
        if (!curCubie.hasInPosition(yellow))
        {
            CubeHandler handler = CubeHandler::genHandler(curCubie, Front, Right, false);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
            _addAndApply(handler, PosMove(Front, false), retVal);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Front, true), retVal);

            curCubie = m_CurrentCubeState.findCubieByColor(piece);
        }

        RASSERT(curCubie.hasInPosition(yellow), "");

        auto isCubieYellowAlligned = [] (const EdgeCube& ec) {
            RubikColor otherThanYellowPosition = ec.getPositionNot(yellow);
            RubikColor otherThanYellowPositionColor = ec.colorForPosition(otherThanYellowPosition);
            return ec.hasInPosition( otherThanYellowPositionColor);
        };
        // make sur it is alligned with the wanted pos. the color not facing yellow must be on the right side.
        for (int nbYellowTurn = 0; !isCubieYellowAlligned(m_CurrentCubeState.findCubieByColor(piece)) && nbYellowTurn<4; nbYellowTurn++)
        {
            _addAndApply(ColMove(yellow, true), retVal);
        }
        RASSERT(isCubieYellowAlligned(m_CurrentCubeState.findCubieByColor(piece)), "");
        curCubie = m_CurrentCubeState.findCubieByColor(piece);

        // Apply one of the two patern, depending on which side it must go.
        // check if we must rotate it cw or ccw.
        CubeHandler handler = CubeHandler::genHandler(curCubie, Front, Up, false);

        RubikColor colorOnYellowFace = curCubie.colorForPosition(yellow);
        Vector3i colorOnYellowFaceVec = getVectorFromColor(colorOnYellowFace);
        Vector3i yellowFaceVec = getVectorFromColor(yellow);
        Vector3f crossProd = colorOnYellowFaceVec.cast<float>().cross(yellowFaceVec.cast<float>());
        Vector3i crossProdi = roundVec(crossProd);

        RubikColor frontColor = handler._getCol(Front);
        Vector3i frontVector = getVectorFromColor(frontColor);

        RASSERT(crossProdi == frontVector || crossProdi == -frontVector, "");
        if (crossProdi != frontVector)
        { // ccw
            _addAndApply(handler, PosMove(Up, false), retVal);
            _addAndApply(handler, PosMove(Left, false), retVal);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Left, true), retVal);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Front, true), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
            _addAndApply(handler, PosMove(Front, false), retVal);

        }
        else
        { //cw
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
            _addAndApply(handler, PosMove(Front, false), retVal);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Front, true), retVal);

        }
        curCubie = m_CurrentCubeState.findCubieByColor(piece);
    }
    RASSERT(wantedCubie == curCubie,"");

    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepTopCross()
{
    ColMoveSeq retVal;

    Cube defCube;
    std::vector<EdgeCube> topLayerEdges;
    std::copy_if(
        defCube.getEdges().begin(), defCube.getEdges().end(), 
        std::back_inserter(topLayerEdges),
        [] (const auto &c) {return (c.hasInColor(yellow)); });

    std::vector< RubikColor > goodOrientation;
    for (int nbTry = 0; goodOrientation.size() < 4 && nbTry < 4; ++nbTry)
    {
        // check the orientation of the yellow edges.
        goodOrientation.clear();
        for (auto &defCubeTopEdge : topLayerEdges)
        {
            EdgeCube curEdge = m_CurrentCubeState.findCubieByPosition(defCubeTopEdge.getPosition());

            if (curEdge.positionForColor(yellow) == yellow)
            {
                goodOrientation.push_back(curEdge.getPositionNot(yellow));
            }
        }
		

        if (goodOrientation.size() == 4)
        {
            break;
        }

        if (goodOrientation.empty())
        {
            CubeHandler handler = CubeHandler::fromTopFront(yellow, red);
            _addAndApply(handler, PosMove(Front, true), retVal);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Front, false), retVal);
        }
        else if (goodOrientation.size() == 2)
        {
            Vector3i goodOrientVector0 = getVectorFromColor(goodOrientation[0]);
            Vector3i goodOrientVector1 = getVectorFromColor(goodOrientation[1]);
            Vector3i crossProd01 = roundVec(goodOrientVector0.cast<float>().cross(goodOrientVector1.cast<float>()));

            if (crossProd01 == Vector3i::Zero()) // the two good orient are oposite cross product?
            {
                CubeHandler handler = CubeHandler::fromTopLeft(yellow, goodOrientation[0]);
                _addAndApply(handler, PosMove(Front, true), retVal);
                _addAndApply(handler, PosMove(Right, true), retVal);
                _addAndApply(handler, PosMove(Up, true), retVal);
                _addAndApply(handler, PosMove(Right, false), retVal);
                _addAndApply(handler, PosMove(Up, false), retVal);
                _addAndApply(handler, PosMove(Front, false), retVal);
            }
            else
            {
                RubikColor leftColorHandle = goodOrientation[(crossProd01.y() > 0) ? 0 : 1];

                CubeHandler handler = CubeHandler::fromTopLeft(yellow, leftColorHandle);

                _addAndApply(handler, PosMove(Front, true), retVal);
                _addAndApply(handler, PosMove(Up, true), retVal);
                _addAndApply(handler, PosMove(Right, true), retVal);
                _addAndApply(handler, PosMove(Up, false), retVal);
                _addAndApply(handler, PosMove(Right, false), retVal);
                _addAndApply(handler, PosMove(Front, false), retVal);

            }
        }
        else
        {
            RASSERT(false,"possible values is 0, 2, or 4.");
        }
    }
    RASSERT(goodOrientation.size() == 4,"");


    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepTopCorners()
{
    ColMoveSeq retVal;

    Cube defCube;
    std::vector<CornerCube> topLayerCorners;
    std::copy_if(
        defCube.getCorners().begin(), defCube.getCorners().end(), 
        std::back_inserter(topLayerCorners),
        [] (const auto &c) {return (c.hasInColor(yellow)); });

    std::vector< CornerCube > goodOrientation;
    std::vector<CornerCube> badOrientation;
    for (int nbTry = 0; goodOrientation.size() < 4 && nbTry < 4; ++nbTry)
    {
        // check the orientation of the yellow edges.
        goodOrientation.clear();
        badOrientation.clear();
        for (auto &defCubeTopCorner: topLayerCorners)
        {
            CornerCube curCorner = m_CurrentCubeState.findCubieByPosition(defCubeTopCorner.getPosition());

            if (curCorner.positionForColor(yellow) == yellow)
            {
                goodOrientation.push_back(curCorner);
            }
            else
            {
                badOrientation.push_back(curCorner);
            }
        }

        if (goodOrientation.size() == 4)
        {
            break;
        }

        // dummy handler. We will replace it anyway
        CubeHandler handler = CubeHandler::fromTopFront(yellow, red);
        if (goodOrientation.empty())
        {
            for (auto &badCorner: badOrientation)
            {
                handler = CubeHandler::genHandler(badCorner, Up , Front, Left);
                RubikColor leftColorPos = handler._getCol(Left);
                RubikColor colorForLeftPos = badCorner.colorForPosition(leftColorPos);
                if (colorForLeftPos == yellow)
                {
                    break;
                }
            }
        }
        else if (goodOrientation.size() == 1)
        {
            handler = CubeHandler::genHandler(goodOrientation[0], Up, Front, Left);
        }
        else // if (goodOrientation.size() == 2)
        {
            for (auto &badCorner: badOrientation)
            {
                handler = CubeHandler::genHandler(badCorner, Up , Front, Left);
                RubikColor leftColorPos = handler._getCol(Front);
                RubikColor colorForLeftPos = badCorner.colorForPosition(leftColorPos);
                if (colorForLeftPos == yellow)
                {
                    break;
                }
            }
        }

        _addAndApply(handler, PosMove(Right, true), retVal);
        _addAndApply(handler, PosMove(Up, true), retVal);
        _addAndApply(handler, PosMove(Right, false), retVal);
        _addAndApply(handler, PosMove(Up, true), retVal);
        _addAndApply(handler, PosMove(Right, true), retVal);
        _addAndApply(handler, PosMove(Up, true), retVal);
        _addAndApply(handler, PosMove(Up, true), retVal);
        _addAndApply(handler, PosMove(Right, false), retVal);
    }

    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepTopCornersPos()
{
    ColMoveSeq retVal;

    Cube defCube;
    std::vector<CornerCube> topLayerCorners;
    std::copy_if(
        defCube.getCorners().begin(), defCube.getCorners().end(), 
        std::back_inserter(topLayerCorners),
        [] (const auto &c) {return (c.hasInColor(yellow)); });

    std::vector< CornerCube > goodOrientation;
    std::vector<CornerCube> badOrientation;
    for (int nbTry = 0; goodOrientation.size() < 4 && nbTry < 4; ++nbTry)
    {
        // check the orientation of the yellow edges.
        goodOrientation.clear();
        badOrientation.clear();
        for (auto &defCubeTopCorner: topLayerCorners)
        {
            CornerCube curCorner = m_CurrentCubeState.findCubieByPosition(defCubeTopCorner.getPosition());

            if (isSameCubeColor(curCorner.getColor(), curCorner.getPosition()))
            {
                goodOrientation.push_back(curCorner);
            }
            else
            {
                badOrientation.push_back(curCorner);
            }
        }

        if (goodOrientation.size() == 4)
        {
            break;
        }

        if (goodOrientation.size() < 2)
        {
            _addAndApply(ColMove(yellow, true), retVal);
        }
        else
        {
            // dummy handler. We will replace it anyway
            CubeHandler handler = CubeHandler::genHandler(goodOrientation[0], Up , Back, Left);
            CornerCube goodCorner1 = goodOrientation[0];
            CornerCube goodCorner2 = goodOrientation[1];
            Vector3i goodCornerVec1 = getVectorFromCoord<3>(goodCorner1.getPosition());
            goodCornerVec1.y() = 0;
            Vector3i goodCornerVec2 = getVectorFromCoord<3>(goodCorner2.getPosition());
            goodCornerVec2.y() = 0;
            Vector3i crossProd01 = roundVec(goodCornerVec1.cast<float>().cross(goodCornerVec2.cast<float>()));


            //if they are diagonaly opposite.
            if (crossProd01 == Vector3i::Zero())
            {
                handler = CubeHandler::genHandler(goodOrientation[0], Up , Back, Left);
            }
            else // they are on one edge.
            {
                //Find the common color pos of the two good corners.
                RubikColor commonColor = noColor;
                for (int i  = 0; i<3; i++)
                {
                    RubikColor curColorPos = goodCorner1.getPosition()[i];
                    if (curColorPos != yellow && goodCorner2.hasInPosition(curColorPos))
                    {
                        commonColor = curColorPos;
                        break;
                    }
                }
                RASSERT(commonColor != noColor, "");
                handler = CubeHandler::fromTopBack(yellow, commonColor);
            }

            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Front, true), retVal);

            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Back, true), retVal);
            _addAndApply(handler, PosMove(Back, true), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);

            _addAndApply(handler, PosMove(Front, false), retVal);
            _addAndApply(handler, PosMove(Right, false), retVal);

            _addAndApply(handler, PosMove(Back, true), retVal);
            _addAndApply(handler, PosMove(Back, true), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);

            _addAndApply(handler, PosMove(Up, false), retVal);

        }
    }


    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepTopEdges()
{
    ColMoveSeq retVal;

    Cube defCube;
    std::vector<EdgeCube> topLayerEdges;
    std::copy_if(
        defCube.getEdges().begin(), defCube.getEdges().end(), 
        std::back_inserter(topLayerEdges),
        [] (const auto &c) {return (c.hasInColor(yellow)); });

    std::vector< RubikColor > goodOrientation;
    for (int nbTry = 0; goodOrientation.size() < 4 && nbTry < 4; ++nbTry)
    {
        // check the orientation of the yellow edges.
        goodOrientation.clear();
        for (auto &defCubeTopEdge : topLayerEdges)
        {
            EdgeCube curEdge = m_CurrentCubeState.findCubieByPosition(defCubeTopEdge.getPosition());

            if (isSameCubeColor(curEdge.getColor(), curEdge.getPosition()))
            {
                goodOrientation.push_back(curEdge.getPositionNot(yellow));
            }
        }

        if (goodOrientation.size() == 4)
        {
            break;
        }

        CubeHandler handler = CubeHandler::fromTopFront(yellow, red);
        bool isCW = true;
        if (goodOrientation.empty())
        {
            handler = CubeHandler::fromTopFront(yellow, red);
        }
        else // if == 1
        {
            RASSERT(goodOrientation.size() == 1, "");

            handler = CubeHandler::fromTopBack(yellow, goodOrientation[0]);
        }

        _addAndApply(handler, PosMove(Front, true), retVal);
        _addAndApply(handler, PosMove(Front, true), retVal);

        _addAndApply(handler, PosMove(Up, isCW), retVal);

        _addAndApply(handler, PosMove(Left, true), retVal);
        _addAndApply(handler, PosMove(Right, false), retVal);
        _addAndApply(handler, PosMove(Front, true), retVal);

        _addAndApply(handler, PosMove(Front, true), retVal);
        _addAndApply(handler, PosMove(Left, false), retVal);
        _addAndApply(handler, PosMove(Right, true), retVal);

        _addAndApply(handler, PosMove(Up, isCW), retVal);

        _addAndApply(handler, PosMove(Front, true), retVal);
        _addAndApply(handler, PosMove(Front, true), retVal);
    }

    return retVal;
}
