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
    void apply(const ColMoveSeq & moves);
    void rotate(RubikColor side, bool clockWise);

    bool isSolved() const {return false;}

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
private:
    static const int NbEdge = 12;
    static const int NbCorner = 8;

    EdgeList edges;
    CornerList corners;
};
