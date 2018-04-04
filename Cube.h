#pragma once

#include "RubikBase.h"
#include "Cubelet.h"
#include <cereal/types/vector.hpp>

class CubeHandler;
class PermutationMap;

class Cube
{
public:
    typedef std::array< EdgeCube, 12 > EdgeList;
    typedef std::array< CornerCube, 8 > CornerList;

    //Default pristine cube.
    Cube();

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( CEREAL_NVP(edges), CEREAL_NVP(corners) ); // serialize things by passing them to the archive
    }


    const EdgeList &getEdges() const {return edges;};
    const CornerList &getCorners() const {return corners;}

    const EdgeCube &getEdge(const EdgeCube& simEdge) const;
    const EdgeCube &getEdge(const EdgeCoord& edgeColor) const;
    const CornerCube &getCorner(const CornerCube& simCorner) const;
    const CornerCube &getCorner(const CornerCoord& cornerColor) const;

    void rotate(const ColMove & move);
    void rotate(const ColMove & move, int nbIter);
    void apply(const ColMoveSeq & moves);
    void rotate(RubikColor side, bool clockWise, int nbIter=1);

    bool isSolved() const;
    bool isCrossSolved(RubikColor col) const;
    bool isValidCube() const;

    // technically does not change the cube, make it easier to compare.
    // void makeCanon();
    //
    // Cube canon() const;

    bool operator!=(const Cube & rhs) const
    {
        return !( *this == rhs);
    }

    bool operator==(const Cube & rhs) const;

    bool diff(const Cube & rhs, EdgePosList &edgeDiff, CornerPosList &cornerDiff);

    const EdgeCube & findCubieByColor(const EdgeCoord & col) const;
    const CornerCube &findCubieByColor(const CornerCoord & col) const;

    const EdgeCube &findCubieByPosition(const EdgeCoord & col) const;
    const CornerCube &findCubieByPosition(const CornerCoord & col) const;

    /////////////////
    // should not be used except for testing.
    void rotateSinglePiece(const CornerCoord & col, bool cw);
    // swap col[0] and col[1] of the col cubie.
    void swapSinglePiece(const CornerCoord & col);
    void flipEdge(const EdgeCoord& col);
    void swapEdges(const EdgeCoord& col1, const EdgeCoord& col2);
    void swapCorners(const CornerCoord& col1, const CornerCoord& col2);

protected:
    EdgeCube & _findCubieByColor(const EdgeCoord & col);
    CornerCube &_findCubieByColor(const CornerCoord & col);

    EdgeCube &_findCubieByPosition(const EdgeCoord & col);
    CornerCube &_findCubieByPosition(const CornerCoord & col);

private:
    PermutationMap * m_PermMapInst;
    EdgeList edges;
    CornerList corners;
};
