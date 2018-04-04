#include "ColorPermutation.h"

namespace
{
    RubikPerm::InternalFunc _defaultFunc()
    {
        RubikPerm::InternalFunc ret(RubikBase::RubikColors.size());
        for (auto i : RubikBase::RubikColors)
            ret[i] = i;
        return ret;
    }

    RubikPerm::InternalFunc _computeInverse(RubikPerm::InternalFunc forwardFunc)
    {
        RubikPerm::InternalFunc ret(RubikBase::RubikColors.size());
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
    forward.resize(RubikBase::RubikColors.size());
    for (auto i : RubikBase::RubikColors)
        forward[i] = forwardFunc[i];
    inverse = _computeInverse(forward);
}
