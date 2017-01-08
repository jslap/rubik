#pragma once

#include <map>

#include "ColorPermutation.h"

//RubikPerm permutationBySide[6] =
//{
 //   RubikPerm(red, green, orange, blue), // white
  //  RubikPerm(red, white, orange, yellow), // blue
//    RubikPerm(blue, white, green, yellow), // orange
//    RubikPerm(red, yellow, orange, white), // green
//    RubikPerm(white, blue, yellow, green), // red
//    RubikPerm(red, blue, orange, green) // yellow
//};

class PermutationMap
{
public:
    static PermutationMap* getInstance();
    static const RubikPerm& permutationBySideRotation(RubikColor col);

protected:
    PermutationMap();

    const RubikPerm& _permutationBySideRotation(RubikColor col) const;

    std::map < RubikColor , RubikPerm > m_PermutationBySideRotation;
};
