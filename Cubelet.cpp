#include "Cubelet.h"
#include "ColorPermMap.h"

namespace {
    // position rotation
    RubikColor rotateCubeletPos(RubikColor RotatingFace, bool clockWise, RubikColor cubeletPosMoving)
    {
        const auto& permToUse = PermutationMap::permutationBySideRotation(RotatingFace);
        if (clockWise)
            return permToUse.nextColor(cubeletPosMoving);
        else
            return permToUse.prevColor(cubeletPosMoving);
    }    
}


template <int cubeDim>
void Cubelet<cubeDim>::rotate(RubikColor side, bool clockWise)
{
    bool isMemeberOfRotatingSide = false;
    for (int i = 0; i< cubeDim && !isMemeberOfRotatingSide; i++)
        isMemeberOfRotatingSide = (side == position[i]);
    if (isMemeberOfRotatingSide)
        for (int i = 0; i< cubeDim; i++)
            position[i] = rotateCubeletPos( side, clockWise, position[i]);
}

template <int cubeDim>
void Cubelet<cubeDim>::makeCanon()
{
    bool contSort = true;
    for(int i = 1; (i <= cubeDim) && contSort; i++)
    {
        contSort = false;
        for (int j=0; j < (cubeDim -1); j++)
        {
            if (position[j+1] < position[j])      // ascending order simply changes to <
            {
                std::swap(position[j+1], position[j]);
                std::swap(color[j+1], color[j]);
                contSort = true;               // indicates that a swap occurred.
            }
        }
    }
}

template class Cubelet< 2 >;
template class Cubelet< 3 >;
