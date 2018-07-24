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

    static const RubikPerm& PermutationBySideRotation(RubikColor col)
    {
        return getInstance()->permutationBySideRotation(col);
    }

    const RubikPerm& permutationBySideRotation(RubikColor col) const
    {
        return m_PermutationBySideRotation[col];
    }

    ~PermutationMap();
    PermutationMap();
protected:


    std::vector < RubikPerm > m_PermutationBySideRotation;
};
