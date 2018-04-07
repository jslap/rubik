#pragma once

#include "RubikBase.h"
#include "Cube.h"

#include <unordered_set>


class GenBruteSolve
{
public:
    typedef std::function<bool(const Cube&)> SolvedCondition;
    typedef std::pair< ColMove , int > MovePlus;
    typedef std::vector<MovePlus> AllowedMoves;

    GenBruteSolve(const SolvedCondition& cond, const AllowedMoves& allowedMoves);
    virtual ~GenBruteSolve() {}

    void setStartingCube(const Cube& c)
    {
        m_CubeToSolve = c;
        m_Solution.clear();
    }

    // clear the results and compute.
    bool solve(int maxMove);
    ColMoveSeq getFullSolution() const;


private:
    const MovePlus& firstMove() const;
    const MovePlus& nextMove(const MovePlus& m) const;

    SolvedCondition m_Condition;
    AllowedMoves    m_AllowedMoves;

    int m_NbSolutionViewed;
    Cube m_CubeToSolve;
    ColMoveSeq m_Solution;

    AllowedMoves    m_GeneratedAllowedMoves;
    std::map<MovePlus, MovePlus> m_NextMoveMap;

    typedef std::array<unsigned char, 8*(3*2 + 1) + 12*(2*2 + 1)> CubeMem;
    struct CubeMemHash {
       size_t operator() (const CubeMem &cm) const;
    };
    void computeCubeMem(const Cube& c, GenBruteSolve::CubeMem& cubeMem);
    std::unordered_map<CubeMem, int, CubeMemHash> m_ProcessedCubes;

};
