#include "GenBruteSolve.h"

#include "RubikAssert.h"

#include <boost/functional/hash.hpp>

namespace
{
    constexpr GenBruteSolve::MovePlus getNoMove() 
    {
        return {{noColor, true}, 0};
    }
}

void applyToCube(Cube& c, const GenBruteSolve::MovePlus& m)
{
    for (int i = 0; i< m.second; i++)
        c.rotate(m.first);
}

bool isAutoReverse(const GenBruteSolve::MovePlus& m)
{
    return m.second == 2;
}

bool isSame_Slow(const GenBruteSolve::MovePlus& m1, const GenBruteSolve::MovePlus& m2)
{
    Cube c1;
    applyToCube(c1, m1);
    Cube c2;
    applyToCube(c2, m2);
    return c1==c2;
}

bool isReverse(const GenBruteSolve::MovePlus& m1, const GenBruteSolve::MovePlus& m2)
{
    // simple cases first so we can fast track.
    if (m1.second == 1 && m2.second == 1)
    {
        return m1.first.first == m2.first.first && m1.first.second != m2.first.second;
    }
    else if (m1.second == 2 && m2.second == 2)
    {
        return m1.first.first == m2.first.first;
    }
    RASSERT(false, "Not implemented for this val.");
    return false;
}

GenBruteSolve::MovePlus reverseMove(const GenBruteSolve::MovePlus& m)
{
    if (isAutoReverse(m))
        return m;
    else if (m.second == 1)
    {
        GenBruteSolve::MovePlus retMove = m;
        retMove.first.second = !retMove.first.second;
        return retMove;
    }
    RASSERT(false, "Not implemented for this val.");
    return getNoMove();
}

GenBruteSolve::GenBruteSolve(const SolvedCondition& cond, const AllowedMoves& allowedMoves):
    m_Condition(cond),
    m_AllowedMoves(allowedMoves)
{
    m_GeneratedAllowedMoves = m_AllowedMoves;

    for (const MovePlus& m: m_AllowedMoves)
    {
        auto addIfNotInList = [] (const MovePlus &m, AllowedMoves& moveList) {
            auto foundRevIter = std::find_if(moveList.begin(), moveList.end(),
                [&m](const MovePlus& other){return isSame_Slow(m, other);});
            if (foundRevIter == moveList.end())
                moveList.push_back(m);
        };
        // add move**-1
        addIfNotInList(reverseMove(m), m_GeneratedAllowedMoves);
        MovePlus m2 = m;
        m2.second = 2;
        addIfNotInList(m2, m_GeneratedAllowedMoves);
    }

    for (int i = 0; i < m_GeneratedAllowedMoves.size()-1; i++)
        m_NextMoveMap[m_GeneratedAllowedMoves[i]] = m_GeneratedAllowedMoves[i+1];
    m_NextMoveMap[m_GeneratedAllowedMoves[m_GeneratedAllowedMoves.size()-1]] = getNoMove();
}

const GenBruteSolve::MovePlus& GenBruteSolve::nextMove(const GenBruteSolve::MovePlus& m) const
{
    auto nextMoveIter = m_NextMoveMap.find(m);
    RASSERT(nextMoveIter != m_NextMoveMap.end(), "Should not happen");
    return nextMoveIter->second;
}
const GenBruteSolve::MovePlus& GenBruteSolve::firstMove() const
{
    return m_GeneratedAllowedMoves.front();
}


ColMoveSeq convertToMove(const GenBruteSolve::AllowedMoves& am)
{
    ColMoveSeq retSol;
    for (const GenBruteSolve::MovePlus& mp: am)
    {
        for (int i = 0; i< mp.second; i++)
            retSol.push_back(mp.first);
    }
    return retSol;
}

size_t GenBruteSolve::CubeMemHash::operator() (const CubeMem &cm) const
{
    std::size_t seed = 378551;
    for (int i = 0 ; i < cm.size()/24; i++)
    {
        std::size_t subseed = 63689;
        for (int j = 0; j<24; j++)
        {
            subseed = 5*subseed + cm.data()[i*24 + j];

        }
        boost::hash_combine(seed, subseed);
       }
    return seed;
}

void GenBruteSolve::computeCubeMem(const Cube& aCube, GenBruteSolve::CubeMem& cubeMem)
{
    RASSERT(sizeof(RubikColor)==1, "");
    RASSERT(sizeof(EdgeCube)==5, "");
    RASSERT(sizeof(CornerCube)==7, "");
    const int sizeOfEdges = sizeof(EdgeCube)*aCube.getEdges().size();
    memcpy(cubeMem.data() , aCube.getEdges().data(), sizeOfEdges);
    memcpy(cubeMem.data()+sizeOfEdges , aCube.getCorners().data(), sizeof(CornerCube)*aCube.getCorners().size());
}

bool GenBruteSolve::solve(int maxMove)
{
    std::cout << "GenBruteSolve::solve: " << maxMove << std::endl;
    m_Solution.clear();
    m_ProcessedCubes.clear();
    m_ProcessedCubes.reserve(300000);
    m_NbSolutionViewed = 0;
    AllowedMoves curMove;
    Cube curCube = m_CubeToSolve;
    CubeMem curCubeMem;

    ////// solve it.
    while (true)
    {
        m_NbSolutionViewed++;
        if (m_NbSolutionViewed%1000000 == 0)
            std::cout << "m_NbSolutionViewed: " << m_NbSolutionViewed << std::endl;

        // check if solved.
        if (m_Condition(curCube))
            break;


        bool cubeIsOKToProc = true;

        if (m_ProcessedCubes.size() < 1e7)
        {
            computeCubeMem(curCube, curCubeMem);
            int& cubeDepthIfExist = m_ProcessedCubes[curCubeMem];
            if (cubeDepthIfExist != 0 && cubeDepthIfExist < curMove.size())
                cubeIsOKToProc = false;
            else
                cubeDepthIfExist = curMove.size();
        }


        if (curMove.size() < maxMove
            && cubeIsOKToProc
            )
        {
            curMove.push_back(firstMove());
            applyToCube(curCube, firstMove());
        }
        else
        {
            MovePlus lastMove = curMove.back();
            MovePlus curNextMove = nextMove(lastMove);

            while (curNextMove == getNoMove())
            {
                applyToCube(curCube, reverseMove(lastMove));
                curMove.pop_back();

                if (curMove.empty())
                    break;

                lastMove = curMove.back();
                curNextMove = nextMove(lastMove);
            }

            if (curMove.empty())
                break;

            applyToCube(curCube, reverseMove(lastMove));
            curMove.pop_back();
            applyToCube(curCube, curNextMove);
            curMove.push_back(curNextMove);
        }

        //

    }

    m_Solution = convertToMove(curMove);
    printf("m_NbSolutionViewed : %d\n", m_NbSolutionViewed);
    for (auto m :curMove)
    {
        std::cout << m.first.first << m.first.second << m.second << "-";
    }
    std::cout << std::endl;
    return true;
}


ColMoveSeq GenBruteSolve::getFullSolution() const
{
    return m_Solution;
}
