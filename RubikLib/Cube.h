#pragma once

#include "RubikAssert.h"
#include "RubikBase.h"
#include "Cubelet.h"

#include "cereal/types/vector.hpp"

class CubeHandler;
class PermutationMap;

class Cube
{
public:
    typedef std::array< EdgeCube, 12 > EdgeList;
    typedef std::array< CornerCube, 8 > CornerList;

    //Default pristine solved cube.
    Cube();
    // Copy
    Cube(const Cube& c);

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
    bool isTopCrossSolved() const {return isCrossSolved(white);}
    bool isValidCube() const;

    bool operator!=(const Cube & rhs) const
    {
        return !( *this == rhs);
    }

    bool operator==(const Cube & rhs) const;

    bool diff(const Cube & rhs, EdgePosList &edgeDiff, CornerPosList &cornerDiff);

    const EdgeCube & findCubieByColor(const typename EdgeCube::_MyCubeCoord & col)  const;
    const CornerCube & findCubieByColor(const typename CornerCube::_MyCubeCoord & col)  const;

    const EdgeCube &findCubieByPosition(const typename EdgeCube::_MyCubeCoord & pos) const;
    const CornerCube &findCubieByPosition(const typename CornerCube::_MyCubeCoord & pos) const;

    /////////////////
    // should not be used except for testing.
    void rotateSinglePiece(const CornerCoord & col, bool cw);
    // swap col[0] and col[1] of the col cubie.
    void swapSinglePiece(const CornerCoord & col);
    void flipEdge(const EdgeCoord& col);
    void swapEdges(const EdgeCoord& col1, const EdgeCoord& col2);
    void swapCorners(const CornerCoord& col1, const CornerCoord& col2);

protected:
    template <class CubieType>
    auto &getCubies()
    {
        if constexpr (std::is_same<CubieType, EdgeCube>::value)
            return edges;
        else if constexpr (std::is_same<CubieType, CornerCube>::value)
            return corners;
        else
            return false; // we request a compile error;
    }

    template <class CubieType>
    CubieType & _findCubieByColor(const typename CubieType::_MyCubeCoord & col);

    template <class CubieType>
    CubieType &_findCubieByPosition(const typename CubieType::_MyCubeCoord & pos);

private:
    PermutationMap * m_PermMapInst;
    EdgeList edges;
    CornerList corners;
};

inline std::ostream& operator<<(std::ostream& out, const Cube& cube) {
    out << "(";
    for (auto c : cube.getCorners())
        out << c << ", ";
    for (auto c : cube.getEdges())
        out << c << ", ";
    out << ")";
   return out;
}
