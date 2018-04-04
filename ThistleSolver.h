#include "CubeSolver.h"


class ThistleSolver: public CubeSolver
{
public:
    ThistleSolver();
    virtual ~ThistleSolver();

    static bool isPhaseOneSolved(const Cube& c);
    static bool isPhaseTwoSolved(const Cube& c);
    static bool isPhaseThreeSolved(const Cube& c);
    void solvePhaseOne();
    void solvePhaseTwo();
    void solvePhaseThree();

protected:
    virtual void computeSolution();
    virtual bool whiteCrossSolveAvail() const {return false;};
    virtual void computeWhiteCross() {}

private:
    Cube m_CurrentCubeState;

};
