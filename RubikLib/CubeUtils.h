#include "RubikAssert.h"

namespace CubeUtils
{
    typedef std::pair<bool, int> NbTurn;
    // return the direction of the turns, and the number of turn.
    //  (true, 1) -> clockwise one time
    //  (true, 0) -> No turn
    template < class CubeOrCubelet, class Condition >
    NbTurn turnUntill(const CubeOrCubelet& c, RubikColor col, Condition f)
    {
        if (f(c))
            return NbTurn(true, 0);
        else
        {
            CubeOrCubelet c1 = c;
            c1.rotate(ColMove(col, true));
            if (f(c1))
                return NbTurn(true, 1);

            c1.rotate(ColMove(col, false), 2);
            if (f(c1))
                return NbTurn(false, 1);
            else
                return NbTurn(false, 2);
        }
    }

    template < class CubeOrCubelet, class Condition >
    CubeOrCubelet turnUntillElem(const CubeOrCubelet& c, RubikColor col, Condition f)
    {
        if (f(c))
            return c;
        else
        {
            CubeOrCubelet c1 = c;
            c1.rotate(ColMove(col, true));
            if (f(c1))
                return c1;

            c1.rotate(ColMove(col, false), 2);
            if (f(c1))
                return c1;
            c1.rotate(ColMove(col, false));
            if (!f(c1))
            {
                Throw("Cant acheive condition");
            }
            return c1;
        }
    }
}
