#include "CubeSolver.h"

class DummyCubeSolver : public CubeSolver
{
public:
    DummyCubeSolver();
    virtual ~DummyCubeSolver();

protected:
    virtual void computeSolution();

private:
    void _addAndApply(ColMove aMove, ColMoveSeq &seq);
    void _addAndApply(const CubeHandler& handler, PosMove aMove, ColMoveSeq &seq);

    ColMoveSeq _solveStepCross() ;
    ColMoveSeq _solveStepCrossElt(RubikColor col);

    ColMoveSeq _solveStepWhiteLayer() ;
    ColMoveSeq _solveStepWhiteLayerElt(const CornerCoord& piece);

    ColMoveSeq _solveStepMiddleLayer() ;
    ColMoveSeq _solveStepMiddleLayerElt(const EdgeCoord& piece);

    ColMoveSeq _solveStepTopCross() ;

    ColMoveSeq _solveStepTopCorners() ;

    ColMoveSeq _solveStepTopCornersPos() ;

    ColMoveSeq _solveStepTopEdges() ;

    Cube m_CurrentCubeState;
};
