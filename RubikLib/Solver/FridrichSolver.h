#include "CubeSolver.h"

#include "Cube.h"

class FridrichCubeSolver : public CubeSolver
{
public:
    FridrichCubeSolver();
    virtual ~FridrichCubeSolver();

protected:
    virtual void computeSolution();
    virtual bool whiteCrossSolveAvail() const {return true;};
    virtual void computeWhiteCross();

    void fastTopCross();
    void bringToCross(EdgeCube c);

    void _addAndApply(RubikColor col, bool direction, int nbTurn);
    void _addAndApply(ColMove aMove);
    void _addAndApply(const ColMoveSeq& seq);
    void _addAndApply(const CubeHandler& handler, PosMove aMove);

private:
    Cube m_CurrentCubeState;
};
