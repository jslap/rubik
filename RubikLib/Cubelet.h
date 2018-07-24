#pragma once

#include "RubikAssert.h"
#include "RubikBase.h"
#include "ColorPermMap.h"
#include "cereal/types/array.hpp"

// A cubelet has 2 or 3 sides.
// It is defined by the color of the facet, that appear on the cubelet, and the position it has in the cube.
// the two facet must be different color.
// the position of a facet is given by the cube face color that the facet is on.
template <int cubeDim>
class Cubelet
{
public:
    typedef CubeCoord< cubeDim > _MyCubeCoord;
    typedef Cubelet< cubeDim > _MyCubelet;

    template<int n>
    friend std::ostream& operator <<(std::ostream& out, const Cubelet<n>& v);

    // invalid cubelet.
    // Cubelet():orientation(WellOriented) {position.fill(noColor); color.fill(noColor);}
    
    // Default cubelet for given color, they are in the "right" position, tha facet color is on the cube face color.
    Cubelet(const _MyCubeCoord& pos):
        position(pos),
        color(pos),
        orientation(WellOriented)
    {
    }
    Cubelet(RubikColor p0, RubikColor p1): // starting pos
        orientation(WellOriented)
    {
        RASSERT(cubeDim == 2, "cubeDim != 2");

        position[0] = color[0] = p0;
        position[1] = color[1] = p1;
    }
    Cubelet(RubikColor p0, RubikColor p1, RubikColor p2) : // starting pos
        orientation(WellOriented)
    {
        RASSERT(cubeDim == 3, "cubeDim != 3");

        position[0] = color[0] = p0;
        position[1] = color[1] = p1;
        position[2] = color[2] = p2;
    }

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( CEREAL_NVP(color), CEREAL_NVP(position) ); // serialize things by passing them to the archive
    }

    bool isSolved() const { return position == color;}

    const _MyCubeCoord &getPosition() const {return position;};
    const _MyCubeCoord &getColor() const {return color;}
    RubikOrientation getOrientation() const {return orientation;}

    // pre-cond: color must be in the cubelet.
    RubikColor positionForColor(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (color[i] == col)
                return position[i];
        RASSERT(false, "Color not in cubie.");
        return noColor;
    }

    RubikColor colorForPosition(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (position[i] == col)
                return color[i];
        RASSERT(false, "Color not in cubie.");
        return noColor;
    }

    RubikColor getColorNot(RubikColor notThisColor) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (color[i] != notThisColor)
                return color[i];
        RASSERT(false, "Color is in all cubie pos..");
        return noColor;
    }

    RubikColor getPositionNot(RubikColor notThisPos) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (position[i] != notThisPos)
                return position[i];
        RASSERT(false, "Color is in all cubie pos..");
        return noColor;
    }

    RubikColor getPositionNot(RubikColor notThisPos, RubikColor orThatPos) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (position[i] != notThisPos && position[i] != orThatPos)
                return position[i];
        RASSERT(false, "Color is in all cubie pos..");
        return noColor;
    }

    RubikColor getColorIn(RubikColor thisPos, RubikColor thatPos) const
    {
        RASSERT(hasInColor(thisPos)||hasInColor(thatPos), "Precondition");
        return _getValIn(thisPos, thatPos, color);
    }

    RubikColor getPositionIn(RubikColor thisPos, RubikColor thatPos) const
    {
        RASSERT(hasInPosition(thisPos)||hasInPosition(thatPos), "Precondition");
        return _getValIn(thisPos, thatPos, position);
    }

    bool isAtSolvedPosition() const
    {
        if (cubeDim == 2)
        {
            return hasInColor(position[0]) && hasInColor(position[1]);
        }
        else if (cubeDim == 3)
        {
            return hasInColor(position[0]) && hasInColor(position[1]) && hasInColor(position[2]);
        }
        RASSERT(false, "Invalid dim");
        return false;
    }

    bool hasInPosition(RubikColor col) const
    {
        if (cubeDim==2)
            return position[0] == col || position[1] == col;
        else if (cubeDim==3)
            return position[0] == col || position[1] == col || position[2] == col;
        RASSERT(false, "Invalid dim");
        return false;
    }

    bool hasInColor(RubikColor col) const
    {
        if (cubeDim==2)
            return color[0] == col || color[1] == col;
        else if (cubeDim==3)
            return color[0] == col || color[1] == col || color[2] == col;
        RASSERT(false, "Invalid dim");
        return false;
    }


    void rotate(const RubikPerm& perm, RubikColor side, bool clockWise, int nbIter = 1);
    void rotate(RubikColor side, bool clockWise, int nbIter = 1);
    void rotate(const ColMove& m, int nbIter = 1) {rotate(m.first, m.second, nbIter);}
    void rotate(const ColMoveSeq & moves) { for (const auto &m: moves) rotate(m.first, m.second);}
    _MyCubelet getRotate(RubikColor side, bool clockWise) const;

    bool operator !=(const  _MyCubelet& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator ==(const  _MyCubelet& rhs) const
    {
        return position==rhs.position && color==rhs.color;
    }

    bool operator< (const  _MyCubelet& rhs) const
    {
        return std::lexicographical_compare(color.begin(), color.end(), rhs.color.begin(), rhs.color.end());
    }

    // should not be used except for testing.
    void rotateSinglePiece(bool cw);
    void swapSinglePiece(RubikColor c1, RubikColor c2);
    void swapPositionWith(_MyCubelet other);
private:
    RubikColor _getValIn(RubikColor thisPos, RubikColor thatPos, _MyCubeCoord toCheck) const
    {
        auto thisIter = std::find(begin(toCheck), end(toCheck), thisPos);
        if (thisIter != end(toCheck))
            return thisPos;
        auto thatIter = std::find(begin(toCheck), end(toCheck), thatPos);
        if (thatIter != end(toCheck))
            return thatPos;
        RASSERT(false, "should not happen.");
        return noColor;
    }

    RubikColor rotateCubeletPos(const RubikPerm& permutation, bool clockWise, RubikColor cubeletPosMoving, int nbIter = 1) const
    {
        if (clockWise)
            return permutation.nextColor(cubeletPosMoving, nbIter);
        else
            return permutation.prevColor(cubeletPosMoving, nbIter);
    }

    void _rotateOrientation(RubikColor side, bool clockWise);


    _MyCubeCoord position;
    _MyCubeCoord color;
    RubikOrientation orientation;
};


typedef Cubelet< 2 >  EdgeCube;
typedef Cubelet< 3 >  CornerCube;

template <>
inline void Cubelet<2>::_rotateOrientation(RubikColor side, bool clockWise)
{
    if (side == white || side == yellow)
        orientation = (RubikOrientation)((orientation+1)%2);
}

template <>
inline void Cubelet<3>::_rotateOrientation(RubikColor side, bool clockWise)
{
    if (side != green && side != blue)
    {
        bool isSpecCorner = (
            (hasInPosition(white) && hasInPosition(blue)) ||
            (hasInPosition(yellow) && hasInPosition(green))
        );
        unsigned char val = (isSpecCorner) ? 1 : 2;
        orientation = (RubikOrientation)((orientation+val)%3);
    }
    // std::cout << *this << std::endl;
}

template <>
inline void Cubelet<2>::rotate(const RubikPerm& permToUse, RubikColor side, bool clockWise, int nbIter /*= 1 */)
{
    if (side == position[0])
    {
        _rotateOrientation(side, clockWise);
        position[1] = rotateCubeletPos( permToUse, clockWise, position[1], nbIter);
    }
    else if (side == position[1])
    {
        _rotateOrientation(side, clockWise);
        position[0] = rotateCubeletPos( permToUse, clockWise, position[0], nbIter);
    }
}

template <>
inline void Cubelet<3>::rotate(const RubikPerm& permToUse, RubikColor side, bool clockWise, int nbIter /*= 1 */)
{
    if (side == position[0])
    {
        _rotateOrientation(side, clockWise);
        position[1] = rotateCubeletPos( permToUse, clockWise, position[1], nbIter);
        position[2] = rotateCubeletPos( permToUse, clockWise, position[2], nbIter);
    }
    else if (side == position[1])
    {
        _rotateOrientation(side, clockWise);
        position[0] = rotateCubeletPos( permToUse, clockWise, position[0], nbIter);
        position[2] = rotateCubeletPos( permToUse, clockWise, position[2], nbIter);
    }
    else if (side == position[2])
    {
        _rotateOrientation(side, clockWise);
        position[0] = rotateCubeletPos( permToUse, clockWise, position[0], nbIter);
        position[1] = rotateCubeletPos( permToUse, clockWise, position[1], nbIter);
    }
}

template <>
inline void Cubelet<2>::rotate(RubikColor side, bool clockWise, int nbIter /*= 1 */)
{
    if (side == position[0])
    {
        const auto& permToUse = PermutationMap::PermutationBySideRotation(side);
        _rotateOrientation(side, clockWise);
        position[1] = rotateCubeletPos( permToUse, clockWise, position[1], nbIter);
    }
    else if (side == position[1])
    {
        const auto& permToUse = PermutationMap::PermutationBySideRotation(side);
        _rotateOrientation(side, clockWise);
        position[0] = rotateCubeletPos( permToUse, clockWise, position[0], nbIter);
    }
}

template <>
inline void Cubelet<3>::rotate(RubikColor side, bool clockWise, int nbIter /*= 1 */)
{
    if (side == position[0])
    {
        const auto& permToUse = PermutationMap::PermutationBySideRotation(side);
        _rotateOrientation(side, clockWise);
        position[1] = rotateCubeletPos( permToUse, clockWise, position[1], nbIter);
        position[2] = rotateCubeletPos( permToUse, clockWise, position[2], nbIter);
    }
    else if (side == position[1])
    {
        const auto& permToUse = PermutationMap::PermutationBySideRotation(side);
        _rotateOrientation(side, clockWise);
        position[0] = rotateCubeletPos( permToUse, clockWise, position[0], nbIter);
        position[2] = rotateCubeletPos( permToUse, clockWise, position[2], nbIter);
    }
    else if (side == position[2])
    {
        const auto& permToUse = PermutationMap::PermutationBySideRotation(side);
        _rotateOrientation(side, clockWise);
        position[0] = rotateCubeletPos( permToUse, clockWise, position[0], nbIter);
        position[1] = rotateCubeletPos( permToUse, clockWise, position[1], nbIter);
    }
}

template <unsigned long n>
inline std::ostream& operator<<(std::ostream& out, const CubeCoord<n>& coord) {
    out << "(";
    for (RubikColor c : coord)
        out << c << ", ";
    out << ")";
   return out;
}

template <int n>
inline std::ostream& operator<<(std::ostream& out, const Cubelet<n>& c) {
   out << "{ col: " << c.color << " - pos: " << c.position << " - " << c.orientation << "}";
   return out;
}
