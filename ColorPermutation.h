#pragma once

#include <map>

#include "RubikBase.h"


class RubikPerm
{
public:
    typedef std::map< RubikColor, RubikColor > Func;

    RubikPerm();
    RubikPerm(const RubikPerm& rhs);

    RubikPerm& operator=(const RubikPerm& rhs);

    void setForward(Func forwardFunc);

    RubikColor nextColor(RubikColor col) const;
    RubikColor prevColor(RubikColor col) const;


private:
    Func forward;
    Func inverse;
};
