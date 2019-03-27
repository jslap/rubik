#include "Cubelet.h"

#include "ColorPermMap.h"

#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/view/zip.hpp>


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
RubikColor Cubelet<cubeDim>::positionForColor(RubikColor col) const
{
    auto zipRange = ranges::view::zip(color, position);
    auto found = ranges::find_if(zipRange, [&col](const auto& aPair){return aPair.first == col;});
    if (found == std::end(zipRange))
    {
        Throw("Color is in all cubie pos..");
    }
    return (*found).second;
}

template <int cubeDim>
RubikColor Cubelet<cubeDim>::colorForPosition(RubikColor col) const
{
    auto zipRange = ranges::view::zip(color, position);
    auto found = ranges::find_if(zipRange, [&col](const auto& aPair){return aPair.second == col;});
    if (found == std::end(zipRange))
    {
        Throw("Color is in all cubie pos..");
    }
    return (*found).first;
}

template <int cubeDim>
RubikColor Cubelet<cubeDim>::getColorNot(RubikColor notThisColor) const
{
    auto found = ranges::find_if(color, [&notThisColor](RubikColor c){return c != notThisColor;});
    if (found == std::end(color))
    {
        Throw("Color is in all cubie pos..");
    }
    return *found;
}

template <int cubeDim>
RubikColor Cubelet<cubeDim>::getPositionNot(RubikColor notThisPos) const
{
    auto found = ranges::find_if(position, [&notThisPos](RubikColor c){return c != notThisPos;});
    if (found == std::end(position))
    {
        Throw("Pos is in all cubie pos..");
    }
    return *found;
}

template <int cubeDim>
RubikColor Cubelet<cubeDim>::getPositionNot(RubikColor notThisPos, RubikColor orThatPos) const
{
    auto found = ranges::find_if(position, [&notThisPos, &orThatPos](RubikColor c){return c != notThisPos && c != orThatPos;});
    if (found == std::end(position))
    {
        Throw("Pos is in all cubie pos..");
    }
    return *found;
}

template <>
void Cubelet<2>::swapSinglePiece(RubikColor /*c1*/, RubikColor /*c2*/)
{
    // orientation will be screwed after this call.
    std::swap(position[0], position[1]);
}

template <>
void Cubelet<3>::swapSinglePiece(RubikColor c1, RubikColor c2)
{
    // orientation will be screwed after this call.
    if ( (color[0] == c1 && color[1] == c2) ||
        (color[1] == c2 && color[0] == c1))
    {
        std::swap(position[0], position[1]);
        }
    else
    {
        std::swap(position[2], position[1]);
    }
}


template class Cubelet< 2 >;
template class Cubelet< 3 >;
