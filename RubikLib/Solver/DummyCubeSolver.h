#include "CubeSolver.h"

#include "Cube.h"

class CubeHandler;
class DummyCubeSolver : public CubeSolver
{
public:
    DummyCubeSolver();
    virtual ~DummyCubeSolver();

    void solveStepCross()
    {
        _resetSolution();
        m_StepSolution.push_back(_solveStepCross());
    }
    ColMoveSeq _solveStepCross();
    static bool isWhiteCrossSolved(const Cube& cube);

    ColMoveSeq _solveStepWhiteLayer() ;
    ColMoveSeq _solveStepWhiteLayerElt(const CornerCoord& piece);

    ColMoveSeq _solveStepMiddleLayer() ;
    ColMoveSeq _solveStepMiddleLayerElt(const EdgeCoord& piece);

    ColMoveSeq _solveStepTopCross() ;

    ColMoveSeq _solveStepTopCorners() ;

    ColMoveSeq _solveStepTopCornersPos() ;

    ColMoveSeq _solveStepTopEdges() ;

protected:
    virtual void computeSolution();
    virtual bool whiteCrossSolveAvail() const;
    virtual void computeWhiteCross();

private:
    void _resetSolution();
    ColMoveSeq _solveStepCrossElt(RubikColor col);

    void _addAndApply(ColMove aMove, ColMoveSeq &seq);
    void _addAndApply(const CubeHandler& handler, PosMove aMove, ColMoveSeq &seq);



    Cube m_CurrentCubeState;
};
