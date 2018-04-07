#include "Cubelet.h"
#include "ColorPermMap.h"


template <int cubeDim>
Cubelet<cubeDim> Cubelet<cubeDim>::getRotate(RubikColor side, bool clockWise) const
{
    _MyCubelet ret = *this;
    ret.rotate(side, clockWise);
    return ret;
}

template <int cubeDim>
void Cubelet<cubeDim>::rotateSinglePiece(bool cw)
{
    // orientation will be screwed after this call.
    if (cubeDim == 2)
    {
        std::swap(position[0], position[1]);
    }
    else
    {
        if (cw)
        {
            std::swap(position[0], position[1]);
            std::swap(position[0], position[2]);
        }
        else
        {
            std::swap(position[0], position[2]);
            std::swap(position[0], position[1]);
        }
    }
}

template <int cubeDim>
void Cubelet<cubeDim>::swapPositionWith(_MyCubelet other)
{
    // orientation will be screwed after this call.
    std::swap(position, other.position);
}


template <int cubeDim>
void Cubelet<cubeDim>::swapSinglePiece(RubikColor c1, RubikColor c2)
{
    // orientation will be screwed after this call.
    if (cubeDim == 2)
        std::swap(position[0], position[1]);
    else
    {
        if ( (color[0] == c1 && color[1] == c2) ||
            (color[1] == c2 && color[0] == c1))
            std::swap(position[0], position[1]);
        else
            std::swap(position[2], position[1]);
    }
}


template class Cubelet< 2 >;
template class Cubelet< 3 >;
