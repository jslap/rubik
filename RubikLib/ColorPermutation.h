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

    RubikColor nextColor(RubikColor col, int nbIter = 1) const;
    RubikColor prevColor(RubikColor col, int nbIter = 1) const;

    typedef std::vector< RubikColor > InternalFunc;
private:
    InternalFunc forward;
    InternalFunc inverse;
};

inline RubikColor RubikPerm::nextColor(RubikColor col, int nbIter /*= 1*/) const
{
    RASSERT(nbIter > 0, "");
    if (nbIter == 1)
        return forward[col];
    else if (nbIter == 2)
        return forward[forward[col]];
    else
    {
        RubikColor retCol = col;
        while ((nbIter--)>0)
            retCol = forward[retCol];
        return retCol;
    }
}

inline RubikColor RubikPerm::prevColor(RubikColor col, int nbIter /*= 1*/) const
{
    RASSERT(nbIter > 0, "");
    if (nbIter == 1)
        return inverse[col];
    else if (nbIter == 2)
        return inverse[inverse[col]];
    else
    {
        RubikColor retCol = col;
        while ((nbIter--)>0)
            retCol = inverse[retCol];
        return retCol;
    }
}
