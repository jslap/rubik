#include "ColorPermutation.h"

namespace
{
    RubikPerm::Func _defaultFunc()
    {
        RubikPerm::Func ret;
        for (auto i : RubikBase::RubikColors)
            ret[i] = i;
        return ret;
    }

    RubikPerm::Func _computeInverse(RubikPerm::Func forwardFunc)
    {
        RubikPerm::Func ret;
        for (auto i : RubikBase::RubikColors)
            ret[forwardFunc[i]] = i;
        return ret;
    }
}

RubikPerm::RubikPerm()
{
    forward = inverse = _defaultFunc();
}

RubikPerm::RubikPerm(const RubikPerm& rhs)
{
    forward = rhs.forward;
    inverse = rhs.inverse;
}

RubikPerm& RubikPerm::operator=(const RubikPerm& rhs)
{
    if (this != &rhs)
    {
        forward = rhs.forward;
        inverse = rhs.inverse;
    }
    return *this;
}

void RubikPerm::setForward(Func forwardFunc)
{
    forward = forwardFunc;
    inverse = _computeInverse(forwardFunc);
}

RubikColor RubikPerm::nextColor(RubikColor col) const
{
    return forward.at(col);
}

RubikColor RubikPerm::prevColor(RubikColor col) const
{
    return inverse.at(col);
}
