#pragma once

#include "RubikBase.h"
#include "Cubelet.h"

class CubeHandler;

class Cube
{
public:
    typedef std::vector< EdgeCube > EdgeList;
    typedef std::vector< CornerCube > CornerList;

    //Default pristine cube.
    Cube();

    const EdgeList &getEdges() const {return edges;};
    const CornerList &getCorners() const {return corners;}

    void rotate(const ColMove & move);
    void rotate(RubikColor side, bool clockWise);

    // technically does not change the cube, make it easier to compare.
    void makeCanon();

    bool operator!=(const Cube & rhs) const
    {
        return !( *this == rhs);
    }

    bool operator==(const Cube & rhs) const;

    bool diff(const Cube & rhs, EdgePosList &edgeDiff, CornerPosList &cornerDiff);

    const EdgeCube & findColorCubePosition(const EdgeCoord & col) const;
    const CornerCube &findColorCubePositionCorner(const CornerCoord & col) const;

    const EdgeCube &findPositionCube(const EdgeCoord & col) const;
    const CornerCube &findPositionCube(const CornerCoord & col) const;

    ColMoveSeq solve(int step) ;

private:
    ColMoveSeq _solveSingleStep(int step) ;

    ColMoveSeq _solveStepCross() ;
    ColMoveSeq _solveStepCrossElt(RubikColor col);

    ColMoveSeq _solveStepWhiteLayer() ;
    ColMoveSeq _solveStepWhiteLayerElt(const CornerCoord& piece);

    ColMoveSeq _solveStepMiddleLayer() ;
    ColMoveSeq _solveStepMiddleLayerElt(const EdgeCoord& piece);

    ColMoveSeq _solveStepTopCross() ;

    ColMoveSeq _solveStepTopCorners() ;

    ColMoveSeq _solveStepTopCornersPos() ;

    ColMoveSeq _solveStepTopEdges() ;

    void _addAndApply(ColMove aMove, ColMoveSeq &seq);
    void _addAndApply(const CubeHandler& handler, PosMove aMove, ColMoveSeq &seq);


    static const int NbEdge = 12;
    static const int NbCorner = 8;

    EdgeList edges;
    CornerList corners;
};
