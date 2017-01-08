#include "Cube.h"
#include "CubeHandler.h"




Cube::Cube()
{
    edges.push_back(Cubelet<2>(orange, blue));
    edges.push_back(Cubelet<2>(orange, white));
    edges.push_back(Cubelet<2>(orange, green));
    edges.push_back(Cubelet<2>(orange, yellow));
    edges.push_back(Cubelet<2>(red, blue));
    edges.push_back(Cubelet<2>(red, white));
    edges.push_back(Cubelet<2>(red, green));
    edges.push_back(Cubelet<2>(red, yellow));

    edges.push_back(Cubelet<2>(yellow, blue));
    edges.push_back(Cubelet<2>(white, blue));
    edges.push_back(Cubelet<2>(yellow, green));
    edges.push_back(Cubelet<2>(white, green));

    corners.push_back(Cubelet<3>(orange, blue, white));
    corners.push_back(Cubelet<3>(orange, blue, yellow));
    corners.push_back(Cubelet<3>(orange, green, white));
    corners.push_back(Cubelet<3>(orange, green, yellow));

    corners.push_back(Cubelet<3>(red, blue, white));
    corners.push_back(Cubelet<3>(red, blue, yellow));
    corners.push_back(Cubelet<3>(red, green, white));
    corners.push_back(Cubelet<3>(red, green, yellow));
}

void Cube::rotate(const ColMove & move)
{
    rotate(move.first, move.second);
}
void Cube::rotate(RubikColor side, bool clockWise)
{
    for (EdgeCube &e: edges)
        e.rotate(side, clockWise);
    for (CornerCube &c: corners)
        c.rotate(side, clockWise);
}

void Cube::makeCanon()
{
    for (EdgeCube &e: edges)
        e.makeCanon();
    std::sort(edges.begin(), edges.end());
    for (CornerCube &c: corners)
        c.makeCanon();
    std::sort(corners.begin(), corners.end());
}

bool Cube::operator==(const Cube & rhs) const
{
    Cube that = *this;
    Cube rhs2 = rhs;
    that.makeCanon();
    rhs2.makeCanon();
    return (that.edges == rhs2.edges) && (that.corners==rhs2.corners);
}


const EdgeCube &Cube::findColorCubePosition(const EdgeCoord & col) const
{
    for (const EdgeCube &e: edges)
        if (isSameCubeColor(e.getColor() , col ))
            return e;
    RASSERT(false, "Cubie not in Cube.");
    return edges[0];
}

const CornerCube &Cube::findColorCubePositionCorner(const CornerCoord & col) const
{
    for (const CornerCube &c: corners)
        if (isSameCubeColor(c.getColor() , col ))
            return c;
    RASSERT(false, "Cubie not in Cube.");
    return corners[0];
}

const EdgeCube &Cube::findPositionCube(const EdgeCoord & col) const
{
    for (const EdgeCube &e: edges)
        if (isSameCubeColor(e.getPosition() , col ))
            return e;
    RASSERT(false, "Cubie not in Cube.");
    return edges[0];
}

const CornerCube &Cube::findPositionCube(const CornerCoord & col) const
{
    for (const CornerCube &c: corners)
        if (isSameCubeColor(c.getPosition() , col ))
            return c;
    RASSERT(false, "Cubie not in Cube.");
    return corners[0];
}

void Cube::_addAndApply(ColMove aMove, ColMoveSeq &seq)
{
    seq.push_back(aMove);
    rotate(aMove);
}

void Cube::_addAndApply(const CubeHandler& handler, PosMove aMove, ColMoveSeq &seq)
{
    _addAndApply(handler.convertMove(aMove), seq);
}


ColMoveSeq Cube::_solveSingleStep(int step)
{
    ColMoveSeq retVal;
    switch (step)
    {
        case 1:
            retVal = _solveStepCross();
            break;
        case 2:
            retVal = _solveStepWhiteLayer();
            break;
        case 3:
            retVal = _solveStepMiddleLayer();
            break;
        case 4:
            retVal = _solveStepTopCross();
            break;
        case 5:
            retVal = _solveStepTopCorners();
            break;
        case 6:
            retVal = _solveStepTopCornersPos();
            break;
        case 7:
            retVal = _solveStepTopEdges();
            break;

    }
    return retVal;
}

ColMoveSeq Cube::_solveStepCrossElt(RubikColor col)
{
    ColMoveSeq retVal;
    //RASSERT(false, "Test");

    RubikColor otherCol = col;
    EdgeCube fisrtElt(white, otherCol);
    fisrtElt.makeCanon();
    EdgeCube curCubie = findColorCubePosition(fisrtElt.getColor());
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
                    Cube cubeCopy = *this;
                    RubikColor otherFacePos = curCubie.getPosition()[0];

                    cubeCopy.rotate(otherFacePos, true);
                    bool clockwiseOk = cubeCopy.findColorCubePosition(fisrtElt.getColor()).hasInPosition(yellow);


                    _addAndApply(ColMove(otherFacePos, clockwiseOk), retVal);
                    _addAndApply(ColMove(yellow, true), retVal);
                    _addAndApply(ColMove(otherFacePos, !clockwiseOk), retVal);

                }
            }

            curCubie = findColorCubePosition(fisrtElt.getColor());
            RASSERT(findColorCubePosition(fisrtElt.getColor()).hasInPosition(yellow), "");

            // turn yellow face untill it is on the right side.
            while (!curCubie.hasInPosition(otherCol))
            {
                _addAndApply(ColMove(yellow, true), retVal);

                curCubie = findColorCubePosition(fisrtElt.getColor());
                curCubie.makeCanon();
            }

            curCubie = findColorCubePosition(fisrtElt.getColor());
            RASSERT(findColorCubePosition(fisrtElt.getColor()).hasInPosition(otherCol) &&
                    findColorCubePosition(fisrtElt.getColor()).hasInPosition(yellow),
                    "");

            // make it on the white side;
            _addAndApply(ColMove(otherCol, true), retVal);
            _addAndApply(ColMove(otherCol, true), retVal);
        }

        curCubie = findColorCubePosition(fisrtElt.getColor());
        RASSERT(findColorCubePosition(fisrtElt.getColor()).hasInPosition(otherCol) && findColorCubePosition(fisrtElt.getColor()).hasInPosition(white),"");

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

        curCubie = findColorCubePosition(fisrtElt.getColor());
        curCubie.makeCanon();
        RASSERT(curCubie == fisrtElt,"");
    }

    return retVal;

}


ColMoveSeq Cube::_solveStepCross()
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

ColMoveSeq Cube::_solveStepWhiteLayer()
{
    ColMoveSeq retVal;

    Cube defCube;
    CornerList whiteLayerCorners;
    for (CornerList::const_iterator it = defCube.getCorners().begin(); it != defCube.getCorners().end(); ++it)
        if (it->hasInColor(white))
            whiteLayerCorners.push_back(*it);

    for (CornerList::const_iterator it = whiteLayerCorners.begin(); it != whiteLayerCorners.end(); ++it)
    {
        ColMoveSeq tmpMoveSeq = _solveStepWhiteLayerElt(it->getColor());
        retVal.insert(retVal.end(), tmpMoveSeq.begin(), tmpMoveSeq.end());
        //if (!retVal.empty()) break;
    }
    return retVal;
}

ColMoveSeq Cube::_solveStepWhiteLayerElt(const CornerCoord& piece)
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

    CornerCube curCubie = findColorCubePositionCorner(piece);
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

            curCubie = findColorCubePositionCorner(piece);
            curCubie.makeCanon();
        }
        RASSERT(curCubie.hasInPosition(yellow), "");

        // allign the cube, in the yellow layer so it is just below the right pos.
        while ( ! (curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]) ) )
        {
            _addAndApply(ColMove(yellow, true), retVal);
            curCubie = findColorCubePositionCorner(piece);
        }

        RASSERT(curCubie.hasInPosition(yellow) && curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]), "");

        {

            // do these steps to try to make it right.
            CubeHandler handler = CubeHandler::genHandler(curCubie, Down, Front, Right);
            _addAndApply(handler, PosMove(Right, false), retVal);
            _addAndApply(handler, PosMove(Down, false), retVal);
            _addAndApply(handler, PosMove(Right, true), retVal);
            _addAndApply(handler, PosMove(Down, true), retVal);

            curCubie = findColorCubePositionCorner(piece);
            curCubie.makeCanon();
        }

        RASSERT(curCubie.hasInPosition(white) && curCubie.hasInPosition(otherColVec[0]) && curCubie.hasInPosition(otherColVec[1]), "");

    }
    return retVal;

}

ColMoveSeq Cube::_solveStepMiddleLayer()
{
    ColMoveSeq retVal;

    Cube defCube;
    EdgeList middleLayerEdges;
    for (EdgeList::const_iterator it = defCube.getEdges().begin(); it != defCube.getEdges().end(); ++it)
        if (!it->hasInColor(white) && !it->hasInColor(yellow))
            middleLayerEdges.push_back(*it);

    for (EdgeList::const_iterator it = middleLayerEdges.begin(); it != middleLayerEdges.end(); ++it)
    {
        ColMoveSeq tmpMoveSeq = _solveStepMiddleLayerElt(it->getColor());
        retVal.insert(retVal.end(), tmpMoveSeq.begin(), tmpMoveSeq.end());
        //if (!retVal.empty()) break;
    }
    return retVal;

}

ColMoveSeq Cube::_solveStepMiddleLayerElt(const EdgeCoord& piece)
{
    //printf("Solve middle layer (%s,%s)\n", colorName(piece[0]).c_str(), colorName(piece[1]).c_str());
    ColMoveSeq retVal;


    EdgeCube wantedCubie(piece);
    wantedCubie.makeCanon();

    EdgeCube curCubie = findColorCubePosition(piece);
    curCubie.makeCanon();

    RubikColor col1 = piece[0];
    RubikColor col2 = piece[1];

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

            curCubie = findColorCubePosition(piece);
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
            curCubie = findColorCubePosition(piece);
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
        curCubie = findColorCubePosition(piece);
        curCubie.makeCanon();

        //RASSERT(wantedCubie == curCubie,"");

    }

    return retVal;
}

ColMoveSeq Cube::_solveStepTopCross()
{
    ColMoveSeq retVal;

    Cube defCube;
    EdgeList topLayerEdges;
    for (EdgeList::const_iterator it = defCube.getEdges().begin(); it != defCube.getEdges().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerEdges.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        std::vector< RubikColor > goodOrientation;
        for (EdgeList::const_iterator it = topLayerEdges.begin(); it != topLayerEdges.end(); ++it)
        {
            EdgeCube curEdge = findPositionCube(it->getPosition());
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

        if (!retVal.empty()) break;

    }


    return retVal;
}

ColMoveSeq Cube::_solveStepTopCorners()
{
    ColMoveSeq retVal;

    Cube defCube;
    CornerList topLayerCorners;
    for (CornerList::const_iterator it = defCube.getCorners().begin(); it != defCube.getCorners().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerCorners.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        CornerList goodOrientation;
        CornerList badOrientation;
        for (CornerList::const_iterator it = topLayerCorners.begin(); it != topLayerCorners.end(); ++it)
        {
            CornerCube curCorner = findPositionCube(it->getPosition());
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
                for (CornerList::const_iterator it = badOrientation.begin(); it != badOrientation.end(); ++it)
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
                for (CornerList::const_iterator it = badOrientation.begin(); it != badOrientation.end(); ++it)
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

ColMoveSeq Cube::_solveStepTopCornersPos()
{
    ColMoveSeq retVal;

    Cube defCube;
    CornerList topLayerCorners;
    for (CornerList::const_iterator it = defCube.getCorners().begin(); it != defCube.getCorners().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerCorners.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        CornerList goodOrientation;
        CornerList badOrientation;
        for (CornerList::const_iterator it = topLayerCorners.begin(); it != topLayerCorners.end(); ++it)
        {
            CornerCube curCorner = findPositionCube(it->getPosition());

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

ColMoveSeq Cube::_solveStepTopEdges()
{
    ColMoveSeq retVal;

    Cube defCube;
    EdgeList topLayerEdges;
    for (EdgeList::const_iterator it = defCube.getEdges().begin(); it != defCube.getEdges().end(); ++it)
        if (it->hasInColor(yellow))
            topLayerEdges.push_back(*it);

    while (1)
    {
        // check the orientation of the yellow edges.
        std::vector< RubikColor > goodOrientation;
        for (EdgeList::const_iterator it = topLayerEdges.begin(); it != topLayerEdges.end(); ++it)
        {
            EdgeCube curEdge = findPositionCube(it->getPosition());
            curEdge.makeCanon();


            RubikColor yellowPos = curEdge.positionForColor(yellow);
            bool isGoodOrient = (yellowPos == yellow);
            RubikColor otherPos = curEdge.getPositionNot(yellow);

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


ColMoveSeq Cube::solve(int step)
{
    ColMoveSeq retVal;
    for (int curStep = 1; curStep<=step; curStep++)
    {
        ColMoveSeq tmp = _solveSingleStep(curStep);
        retVal.insert(retVal.end(), tmp.begin(), tmp.end());
        if (!retVal.empty())
            break;
    }
    return retVal;
}


bool Cube::diff(const Cube & rhs, EdgePosList &edgeDiff, CornerPosList &cornerDiff)
{
    edgeDiff.clear();
    cornerDiff.clear();

    Cube that = *this;
    Cube rhs2 = rhs;
    that.makeCanon();
    rhs2.makeCanon();

    EdgeList::iterator eIter = that.edges.begin();
    EdgeList::iterator eIterRhs = rhs2.edges.begin();
    for( ; eIter != that.edges.end(); ++eIter, ++eIterRhs)
        if (eIter->getColor() != eIterRhs->getColor())
            edgeDiff.push_back(eIter->getPosition());

    CornerList::iterator cornIter = that.corners.begin();
    CornerList::iterator cornIterRhs = rhs2.corners.begin();
    for( ; cornIter != that.corners.end(); ++cornIter, ++cornIterRhs)
        if (cornIter->getColor() != cornIterRhs->getColor())
            cornerDiff.push_back(cornIter->getPosition());

    return edgeDiff.empty() && cornerDiff.empty();
}
