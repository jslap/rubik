#include "DummyCubeSolver.h"
#include "CubeHandler.h"

DummyCubeSolver::DummyCubeSolver()
{
}

 DummyCubeSolver::~DummyCubeSolver()
 {
 }

void DummyCubeSolver::computeSolution()
{
    m_CurrentCubeState = m_CubeToSolve;

    ColMoveSeq stepSol;
    stepSol = _solveStepCross();
    if (!stepSol.empty())
        m_StepSolution.push_back(stepSol);
    stepSol = _solveStepWhiteLayer();
    if (!stepSol.empty())
        m_StepSolution.push_back(stepSol);
    stepSol = _solveStepMiddleLayer();
    if (!stepSol.empty())
        m_StepSolution.push_back(stepSol);
    stepSol = _solveStepTopCross();
    if (!stepSol.empty())
        m_StepSolution.push_back(stepSol);
    stepSol = _solveStepTopCorners();
    if (!stepSol.empty())
        m_StepSolution.push_back(stepSol);
    stepSol = _solveStepTopCornersPos();
    if (!stepSol.empty())
        m_StepSolution.push_back(stepSol);
    stepSol = _solveStepTopEdges();
    if (!stepSol.empty())
        m_StepSolution.push_back(stepSol);
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
    ColMoveSeq retVal;

    RubikColor otherCol = col;
    EdgeCube fisrtElt(white, otherCol);
    fisrtElt.makeCanon();
    EdgeCube curCubie = m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor());
    curCubie.makeCanon();
    if (curCubie != fisrtElt)
    {
        if (curCubie.hasInPosition(white) && curCubie.hasInPosition(otherCol))
        {

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
                    bool clockwiseOk = cubeCopy.findColorCubePosition(fisrtElt.getColor()).hasInPosition(yellow);


                    _addAndApply(ColMove(otherFacePos, clockwiseOk), retVal);
                    _addAndApply(ColMove(yellow, true), retVal);
                    _addAndApply(ColMove(otherFacePos, !clockwiseOk), retVal);

                }
            }

            curCubie = m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor());
            RASSERT(m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor()).hasInPosition(yellow), "");

            // turn yellow face untill it is on the right side.
            while (!curCubie.hasInPosition(otherCol))
            {
                _addAndApply(ColMove(yellow, true), retVal);

                curCubie = m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor());
                curCubie.makeCanon();
            }

            curCubie = m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor());
            RASSERT(m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor()).hasInPosition(otherCol) &&
                    m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor()).hasInPosition(yellow),
                    "");

            // make it on the white side;
            _addAndApply(ColMove(otherCol, true), retVal);
            _addAndApply(ColMove(otherCol, true), retVal);
        }

        curCubie = m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor());
        RASSERT(m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor()).hasInPosition(otherCol) &&
            m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor()).hasInPosition(white),"");

        //if (!retVal.empty()) return retVal;
        curCubie.makeCanon();
        if (curCubie != fisrtElt)
        {
            CubeHandler handler = CubeHandler::fromTopRight(white, otherCol);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Up, true), retVal);
            _addAndApply(handler, PosMove(Front, false), retVal);
            _addAndApply(handler, PosMove(Up, false), retVal);
        }

        curCubie = m_CurrentCubeState.findColorCubePosition(fisrtElt.getColor());
        curCubie.makeCanon();
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
    Cube::CornerList whiteLayerCorners;
    for (Cube::CornerList::const_iterator it = defCube.getCorners().begin(); it != defCube.getCorners().end(); ++it)
        if (it->hasInColor(white))
            whiteLayerCorners.push_back(*it);

    for (Cube::CornerList::const_iterator it = whiteLayerCorners.begin(); it != whiteLayerCorners.end(); ++it)
    {
        ColMoveSeq tmpMoveSeq = _solveStepWhiteLayerElt(it->getColor());
        retVal.insert(retVal.end(), tmpMoveSeq.begin(), tmpMoveSeq.end());
    }
    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepWhiteLayerElt(const CornerCoord& piece)
{
    //printf("Solve white corner (%s,%s,%s)\n", colorName(piece[0]).c_str(), colorName(piece[1]).c_str(), colorName(piece[2]).c_str());
    ColMoveSeq retVal;

    // get the other colors of the piece: other than white.
    std::vector< RubikColor > otherColVec;
    for (int i = 0; i<piece.size(); i++)
        if (piece[i] != white)
            otherColVec.push_back(piece[i]);
    RASSERT(otherColVec.size() == 2, "");

    CornerCube wantedCubie(piece);
    wantedCubie.makeCanon();

    CornerCube curCubie = m_CurrentCubeState.findColorCubePositionCorner(piece);
    curCubie.makeCanon();

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

            curCubie = m_CurrentCubeState.findColorCubePositionCorner(piece);
            curCubie.makeCanon();
        }
        RASSERT(curCubie.hasInPosition(yellow), "");

        // allign the cube, in the yellow layer so it is just below the right pos.
        while ( ! (curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]) ) )
        {
            _addAndApply(ColMove(yellow, true), retVal);
            curCubie = m_CurrentCubeState.findColorCubePositionCorner(piece);
        }

        RASSERT(curCubie.hasInPosition(yellow) && curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]), "");

        {

            // do these steps to try to make it right.
            CubeHandler handler = CubeHandler::genHandler(curCubie, Down, Front, Right);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Down, false), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);
            _addAndApply(handler, PosMove(Down, true), retVal);

            curCubie = m_CurrentCubeState.findColorCubePositionCorner(piece);
            curCubie.makeCanon();
        }

        RASSERT(curCubie.hasInPosition(white) && curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]), "");

    }
    return retVal;

}

ColMoveSeq DummyCubeSolver::_solveStepMiddleLayer()
{
    ColMoveSeq retVal;

    Cube defCube;
    Cube::EdgeList middleLayerEdges;
    for (Cube::EdgeList::const_iterator it = defCube.getEdges().begin(); it != defCube.getEdges().end(); ++it)
        if (!it->hasInColor(white) && !it->hasInColor(yellow))
            middleLayerEdges.push_back(*it);

    for (Cube::EdgeList::const_iterator it = middleLayerEdges.begin(); it != middleLayerEdges.end(); ++it)
    {
        ColMoveSeq tmpMoveSeq = _solveStepMiddleLayerElt(it->getColor());
        retVal.insert(retVal.end(), tmpMoveSeq.begin(), tmpMoveSeq.end());
    }
    return retVal;

}

ColMoveSeq DummyCubeSolver::_solveStepMiddleLayerElt(const EdgeCoord& piece)
{
    //printf("Solve middle layer (%s,%s)\n", colorName(piece[0]).c_str(), colorName(piece[1]).c_str());
    ColMoveSeq retVal;


    EdgeCube wantedCubie(piece);
    wantedCubie.makeCanon();

    EdgeCube curCubie = m_CurrentCubeState.findColorCubePosition(piece);
    curCubie.makeCanon();

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

            curCubie = m_CurrentCubeState.findColorCubePosition(piece);
            curCubie.makeCanon();
        }

        RASSERT(curCubie.hasInPosition(yellow), "");

        // make sur it is alligned with the wanted pos. the color not facing yellow must be on the right side.
        do
        {
            RubikColor otherThanYellowPosition = curCubie.getPositionNot(yellow);
            RubikColor otherThanYellowPositionColor = curCubie.colorForPosition(otherThanYellowPosition);
            bool isAlligned = curCubie.hasInPosition( otherThanYellowPositionColor);

            if (isAlligned)
                break;

            _addAndApply(ColMove(yellow, true), retVal);
            curCubie = m_CurrentCubeState.findColorCubePosition(piece);
            curCubie.makeCanon();

        } while (1);

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
        curCubie = m_CurrentCubeState.findColorCubePosition(piece);
        curCubie.makeCanon();

        //RASSERT(wantedCubie == curCubie,"");

    }

    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepTopCross()
{
    ColMoveSeq retVal;

    Cube defCube;
    Cube::EdgeList topLayerEdges;
    for (Cube::EdgeList::const_iterator it = defCube.getEdges().begin(); it != defCube.getEdges().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerEdges.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        std::vector< RubikColor > goodOrientation;
        for (Cube::EdgeList::const_iterator it = topLayerEdges.begin(); it != topLayerEdges.end(); ++it)
        {
            EdgeCube curEdge = m_CurrentCubeState.findPositionCube(it->getPosition());
            curEdge.makeCanon();


            RubikColor yellowPos = curEdge.positionForColor(yellow);
            bool isGoodOrient = (yellowPos == yellow);
            RubikColor otherPos = curEdge.getPositionNot(yellow);
            if (isGoodOrient) goodOrientation.push_back(otherPos);
        }

        if (goodOrientation.size() == 4)
            break;
        else if (goodOrientation.size() == 0)
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
                RubikColor leftColorHandle;
                if (crossProd01.y() > 0)
                    leftColorHandle = goodOrientation[0];
                else
                    leftColorHandle = goodOrientation[1];

                CubeHandler handler = CubeHandler::fromTopLeft(yellow, leftColorHandle);

                _addAndApply(handler, PosMove(Front, true), retVal);
                _addAndApply(handler, PosMove(Up, true), retVal);
                _addAndApply(handler, PosMove(Right, true), retVal);
                _addAndApply(handler, PosMove(Up, false), retVal);
                _addAndApply(handler, PosMove(Right, false), retVal);
                _addAndApply(handler, PosMove(Front, false), retVal);

            }
        }
    }


    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepTopCorners()
{
    ColMoveSeq retVal;

    Cube defCube;
    Cube::CornerList topLayerCorners;
    for (Cube::CornerList::const_iterator it = defCube.getCorners().begin(); it != defCube.getCorners().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerCorners.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        Cube::CornerList goodOrientation;
        Cube::CornerList badOrientation;
        for (Cube::CornerList::const_iterator it = topLayerCorners.begin(); it != topLayerCorners.end(); ++it)
        {
            CornerCube curCorner = m_CurrentCubeState.findPositionCube(it->getPosition());
            curCorner.makeCanon();


            RubikColor yellowPos = curCorner.positionForColor(yellow);
            bool isGoodOrient = (yellowPos == yellow);
            if (isGoodOrient) goodOrientation.push_back(curCorner);
            else badOrientation.push_back(curCorner);
        }

        if (goodOrientation.size() == 4)
            break;
        else
        {
            // dummy handler. We will replace it anyway
            CubeHandler handler = CubeHandler::fromTopFront(yellow, red);
            if (goodOrientation.size() == 0)
            {
                for (Cube::CornerList::const_iterator it = badOrientation.begin(); it != badOrientation.end(); ++it)
                {
                    CornerCube curCorner = *it;
                    handler = CubeHandler::genHandler(curCorner, Up , Front, Left);
                    RubikColor leftColorPos = handler._getCol(Left);
                    RubikColor colorForLeftPos = curCorner.colorForPosition(leftColorPos);
                    if (colorForLeftPos == yellow)
                        break;
                }
            }
            else if (goodOrientation.size() == 1)
            {
                handler = CubeHandler::genHandler(goodOrientation[0], Up, Front, Left);
            }
            else // if (goodOrientation.size() == 2)
            {
                for (Cube::CornerList::const_iterator it = badOrientation.begin(); it != badOrientation.end(); ++it)
                {
                    CornerCube curCorner = *it;
                    handler = CubeHandler::genHandler(curCorner, Up , Front, Left);
                    RubikColor leftColorPos = handler._getCol(Front);
                    RubikColor colorForLeftPos = curCorner.colorForPosition(leftColorPos);
                    if (colorForLeftPos == yellow)
                        break;
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
    }


    return retVal;
}

ColMoveSeq DummyCubeSolver::_solveStepTopCornersPos()
{
    ColMoveSeq retVal;

    Cube defCube;
    Cube::CornerList topLayerCorners;
    for (Cube::CornerList::const_iterator it = defCube.getCorners().begin(); it != defCube.getCorners().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerCorners.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        Cube::CornerList goodOrientation;
        Cube::CornerList badOrientation;
        for (Cube::CornerList::const_iterator it = topLayerCorners.begin(); it != topLayerCorners.end(); ++it)
        {
            CornerCube curCorner = m_CurrentCubeState.findPositionCube(it->getPosition());

            if (isSameCubeColor(curCorner.getColor(), curCorner.getPosition()))
                goodOrientation.push_back(curCorner);
            else
                badOrientation.push_back(curCorner);
        }

        if (goodOrientation.size() == 4)
            break;
        else if (goodOrientation.size() < 2)
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
    Cube::EdgeList topLayerEdges;
    for (Cube::EdgeList::const_iterator it = defCube.getEdges().begin(); it != defCube.getEdges().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerEdges.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        std::vector< RubikColor > goodOrientation;
        for (Cube::EdgeList::const_iterator it = topLayerEdges.begin(); it != topLayerEdges.end(); ++it)
        {
            EdgeCube curEdge = m_CurrentCubeState.findPositionCube(it->getPosition());
            curEdge.makeCanon();

            if (isSameCubeColor(curEdge.getColor(), curEdge.getPosition()))
                goodOrientation.push_back(curEdge.getPositionNot(yellow));
        }

        if (goodOrientation.size() == 4)
            break;
        else
        {
            CubeHandler handler = CubeHandler::fromTopFront(yellow, red);
            bool isCW = true;
            if (goodOrientation.size() == 0)
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
    }

    return retVal;
}
