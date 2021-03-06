#include "CubeHandler.h"

#include "RubikAssert.h"

using Eigen::Vector3f;
using Eigen::Matrix3f;

namespace
{
    RubikFace getDefaultPositionFaceFromColor(RubikColor col)
    {
        return static_cast<RubikFace> (col);
    } 

} // namespace

void CubeHandler::generateFromMatrix(const Matrix3f &rotMat)
{
    for (auto faceCol : RubikBase::RubikColors())
    {
        Vector3f curColVec = getVectorFromColor(faceCol).cast<float>();
        Vector3f transVec = rotMat*curColVec;
        RubikColor dstCol = getColorFromVecf(transVec);
        RubikFace curColorFaceEquiv = getDefaultPositionFaceFromColor(faceCol);
        _posToColorMap[curColorFaceEquiv] = dstCol;
    }
}

CubeHandler CubeHandler::fromTopLeft(RubikColor topFace, RubikColor leftFace)
{
    CubeHandler handler;
    Vector3f topColPos = getVectorFromColor(topFace).cast<float>();
    Vector3f leftColPos = getVectorFromColor(leftFace).cast<float>();

    // generate rotation so first color is x and second color is y.
    Matrix3f rotMat;
    rotMat.col(1) = topColPos;
    rotMat.col(0) = -leftColPos;
    rotMat.col(2) = (-leftColPos).cross(topColPos);

    handler.generateFromMatrix(rotMat);

    return handler;
}

CubeHandler CubeHandler::fromTopFront(RubikColor topFace, RubikColor frontFace)
{
    CubeHandler handler;
    Vector3f topColPos = getVectorFromColor(topFace).cast<float>();
    Vector3f frontColPos = getVectorFromColor(frontFace).cast<float>();

    // generate rotation so first color is x and second color is y.
    Matrix3f rotMat;
    rotMat.col(1) = topColPos;
    rotMat.col(2) = frontColPos;
    rotMat.col(0) = topColPos.cross(frontColPos);

    handler.generateFromMatrix(rotMat);

    return handler;
}

CubeHandler CubeHandler::fromTopBack(RubikColor topFace, RubikColor backFace)
{
    CubeHandler handler;
    Vector3f topColPos = getVectorFromColor(topFace).cast<float>();
    Vector3f backColPos = getVectorFromColor(backFace).cast<float>();

    // generate rotation so first color is x and second color is y.
    Matrix3f rotMat;
    rotMat.col(1) = topColPos;
    rotMat.col(2) = -backColPos;
    rotMat.col(0) = topColPos.cross(-backColPos);

    handler.generateFromMatrix(rotMat);

    return handler;
}


CubeHandler CubeHandler::fromTopRight(RubikColor topFace, RubikColor rightFace)
{
    CubeHandler handler;
    Vector3f topColPos = getVectorFromColor(topFace).cast<float>();
    Vector3f rightColPos = getVectorFromColor(rightFace).cast<float>();

    // generate rotation so first color is x and second color is y.
    Matrix3f rotMat;
    rotMat.col(1) = topColPos;
    rotMat.col(0) = rightColPos;
    rotMat.col(2) = rightColPos.cross(topColPos);

    handler.generateFromMatrix(rotMat);
    return handler;
}

CubeHandler::CubeHandler() = default;

void CubeHandler::rotate(Cube &cube, RubikFace face, bool cw) const
{
    cube.rotate(_getCol(face), cw);
}

ColMoveSeq CubeHandler::convertSeq(const PosMoveSeq & posSeq) const
{
    ColMoveSeq retVal;
    transform(posSeq.begin(), posSeq.end(), back_inserter(retVal), [this] (const PosMove& p) {return convertMove(p);});
    return retVal;
}

ColMove CubeHandler::convertMove(PosMove posMove) const
{
    return ColMove(_getCol(posMove.first), posMove.second);
}


RubikColor CubeHandler::_getCol(RubikFace f) const
{
    auto it = _posToColorMap.find(f);
    return it==_posToColorMap.end() ? noColor : it->second;
}

const EdgeCube& CubeHandler::getCubie(const Cube &cube, RubikFace face1, RubikFace face2) const
{
    EdgeCube edgeCube(_getCol(face1), _getCol(face2));
    EdgeCoord edgeCoord = edgeCube.getPosition();
    return cube.findCubieByPosition(edgeCoord);
}

const CornerCube& CubeHandler::getCubie(const Cube &cube, RubikFace face1, RubikFace face2, RubikFace face3) const
{
    CornerCube cornerCube(_getCol(face1), _getCol(face2), _getCol(face3));
    CornerCoord cornerCoord = cornerCube.getPosition();
    return cube.findCubieByPosition(cornerCoord);
}

std::list< CubeHandler > CubeHandler::genHandlerList;
bool CubeHandler::listIsInit = false;
void CubeHandler::_generateAllHandler()
{
    // TODO(js) find a less ugly way to do that.
    if (!listIsInit)
    {
        genHandlerList.push_back(CubeHandler::fromTopFront(white, red));
        genHandlerList.push_back(CubeHandler::fromTopFront(white, blue));
        genHandlerList.push_back(CubeHandler::fromTopFront(white, orange));
        genHandlerList.push_back(CubeHandler::fromTopFront(white, green));

        genHandlerList.push_back(CubeHandler::fromTopFront(yellow, red));
        genHandlerList.push_back(CubeHandler::fromTopFront(yellow, blue));
        genHandlerList.push_back(CubeHandler::fromTopFront(yellow, orange));
        genHandlerList.push_back(CubeHandler::fromTopFront(yellow, green));
        listIsInit = true;
    }
}

CubeHandler CubeHandler::genHandler(CornerCube cubie, RubikFace face1, RubikFace face2, RubikFace face3)
{
    _generateAllHandler();

    for (const auto &handler: genHandlerList )
    {
        Cube defaultCube;
        CornerCube cube1 = handler.getCubie(defaultCube, face1, face2, face3);
        if (isSameCubeColor(cube1.getPosition(), cubie.getPosition()))
        {
            return handler;
        }
    }
    RASSERT(false, "");
    return genHandlerList.front();
}

CubeHandler CubeHandler::genHandler(EdgeCube cubie, RubikFace face1, RubikFace face2, bool whiteOnTop)
{
    _generateAllHandler();

    for (const auto &handler: genHandlerList )
    {
        Cube aCube;
        EdgeCube cube1 = handler.getCubie(aCube, face1, face2);
        bool cubeIsTheRightOne = (isSameCubeColor(cube1.getPosition(), cubie.getPosition()));
        if (cubeIsTheRightOne)
        {
            auto topColorIter = handler._posToColorMap.find(Up);
            bool whiteIsOnTopForHandler =  (topColorIter != handler._posToColorMap.end() && (topColorIter->second == white));
            if (whiteIsOnTopForHandler == whiteOnTop)
            {
                return handler;
            }
        }
    }
    RASSERT(false, "");
    return genHandlerList.front();
}



PosMoveSeq CubeHandler::genSeq()
{
    PosMoveSeq retVal;
    retVal.push_back(PosMove(Front, true));
    retVal.push_back(PosMove(Front, true));
    retVal.push_back(PosMove(Up, true));
    retVal.push_back(PosMove(Left, true));
    retVal.push_back(PosMove(Right, false));
    retVal.push_back(PosMove(Front, true));

    retVal.push_back(PosMove(Front, true));
    retVal.push_back(PosMove(Left, false));
    retVal.push_back(PosMove(Right, true));
    retVal.push_back(PosMove(Up, true));
    retVal.push_back(PosMove(Front, true));
    retVal.push_back(PosMove(Front, true));
    return retVal;
}
