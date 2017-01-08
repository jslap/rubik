#pragma once

#include "RubikBase.h"

// A cubelet has 2 or 3 sides.
// It is defined by the color of the facet, that appear on the cubelet, and the position it has in the cube.
// the two facet must be different color.
// the position of a facet is given by the cube face color that the facet is on.
template <int cubeDim>
class Cubelet
{
public:
    // Default cubelet for given color, they are in the "right" position, tha facet color is on the cube face color.
    Cubelet(const CubeCoord< cubeDim >& pos)
    {
        position = pos;
        color = pos;
    }
    Cubelet(RubikColor p0, RubikColor p1) // starting pos
    {
        RASSERT(cubeDim == 2, "cubeDim != 2");

        position[0] = color[0] = p0;
        position[1] = color[1] = p1;
    }
    Cubelet(RubikColor p0, RubikColor p1, RubikColor p2) // starting pos
    {
        RASSERT(cubeDim == 3, "cubeDim != 3");

        position[0] = color[0] = p0;
        position[1] = color[1] = p1;
        position[2] = color[2] = p2;
    }

    const CubeCoord< cubeDim > &getPosition() const {return position;};
    const CubeCoord< cubeDim > &getColor() const {return color;}

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

    bool hasInPosition(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (position[i] == col)
                return true;
        return false;
    }

    bool hasInColor(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (color[i] == col)
                return true;
        return false;
    }


    void rotate(RubikColor side, bool clockWise);

    void makeCanon();

    bool operator !=(const  Cubelet<cubeDim>& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator ==(const  Cubelet<cubeDim>& rhs) const
    {
        return position==rhs.position && color==rhs.color;
    }

    bool operator< (const  Cubelet<cubeDim>& rhs) const
    {
        return std::lexicographical_compare(position.begin(), position.end(), rhs.position.begin(), rhs.position.end());
    }
private:

    CubeCoord< cubeDim > position;
    CubeCoord< cubeDim > color;
};


typedef Cubelet< 2 >  EdgeCube;
typedef Cubelet< 3 >  CornerCube;
