#include "ExportCubeSolver.h"

#include "Cube.h"


/**********************************************************************
 * 
 * A cube 'state' is a vector<int> with 40 entries, the first 20
 * are a permutation of {0,...,19} and describe which cubie is at
 * a certain position (regarding the input ordering). The first
 * twelve are for edges, the last eight for corners.
 * 
 * The last 20 entries are for the orientations, each describing
 * how often the cubie at a certain position has been turned
 * counterclockwise away from the correct orientation. Again the
 * first twelve are edges, the last eight are corners. The values
 * are 0 or 1 for edges and 0, 1 or 2 for corners.
 * 
 **********************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>


namespace
{
constexpr int applicableMoves[] = { 0, 262143, 259263, 74943, 74898 };


int inverse ( int move ) {
  return move + 2 - 2 * (move % 3);
}
} //namespace


using ExportSolverConvert::CubeVecInt;

CubeVecInt ExportCubeSolver::computeId ( CubeVecInt state ) 
{
  
    //--- Phase 1: Edge orientations.
    if( phase < 2 )
    {
        return CubeVecInt( state.begin() + 20, state.begin() + 32 );
    }
  
    //-- Phase 2: Corner orientations, E slice edges.
    if( phase < 3 )
    {
        CubeVecInt result( state.begin() + 31, state.begin() + 40 );
        for( int e=0; e<12; e++ )
        {
            result[0] |= (state[e] / 8) << e;
        }
        return result;
    }
  
  //--- Phase 3: Edge slices M and S, corner tetrads, overall parity.
    if( phase < 4 )
    {
        CubeVecInt result( 3 );
        for( int e=0; e<12; e++ )
        {
            result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2*e);
        }
        for( int c=0; c<8; c++ )
        {
            result[1] |= ((state[c+12]-12) & 5) << (3*c);
        }
        for( int i=12; i<20; i++ )
        {
            for( int j=i+1; j<20; j++ )
            {
                result[2] ^= state[i] > state[j];
            }
        }
        return result;
    }

    //--- Phase 4: The rest.
    return state;
}

//----------------------------------------------------------------------

ColMoveSeq ExportCubeSolver::dosolve( const CubeVecInt startState ) 
{
    ColMoveSeq outMoves;

    //--- Prepare current (start) and goal state.
    CubeVecInt currentState = startState;
    CubeVecInt goalState( 40 );
    for( int i=0; i<20; i++ )
    {
        //--- Goal state.
        goalState[i] = i;  
    }
  
    //--- Dance the funky Thistlethwaite...
    while( ++phase < 5 )
    {
    
        //--- Compute ids for current and goal state, skip phase if equal.
        CubeVecInt currentId = computeId( currentState ), goalId = computeId( goalState );
        if( currentId == goalId )
        continue;
        
        //--- Initialize the BFS queue.
        std::queue<CubeVecInt> q;
        q.push( currentState );
        q.push( goalState );
        
        //--- Initialize the BFS tables.
        std::map<CubeVecInt,CubeVecInt> predecessor;
        std::map<CubeVecInt,int> direction, lastMove;
        direction[ currentId ] = 1;
        direction[ goalId ] = 2;
        
        //--- Dance the funky bidirectional BFS...
        while( 1 )
        {
        
            //--- Get state from queue, compute its ID and get its direction.
            CubeVecInt oldState = q.front();
            q.pop();
            CubeVecInt oldId = computeId( oldState );
            int& oldDir = direction[oldId];
        
            //--- Apply all applicable moves to it and handle the new state.
            for( int move=0; move<18; move++ )
            {
                if( applicableMoves[phase] & (1 << move) )
                {
        
                    //--- Apply the move.
                    CubeVecInt newState = ExportSolverConvert::applyMove( move, oldState );
                    CubeVecInt newId = computeId( newState );
                    int& newDir = direction[newId];
        
                    //--- Have we seen this state (computeId) from the other direction already?
                    //--- I.e. have we found a connection?
                    if( newDir  &&  newDir != oldDir )
                    {
            
                        //--- Make oldId represent the forwards and newId the backwards search state.
                        if( oldDir > 1 )
                        {
                            swap( newId, oldId );
                            move = inverse( move );
                        }
            
                        //--- Reconstruct the connecting algorithm.
                        CubeVecInt algorithm( 1, move );
                        while( oldId != currentId )
                        {
                            algorithm.insert( algorithm.begin(), lastMove[ oldId ] );
                            oldId = predecessor[ oldId ];
                        }
                        while( newId != goalId )
                        {
                            algorithm.push_back( inverse( lastMove[ newId ] ));
                            newId = predecessor[ newId ];
                        }
            
                        //--- Print and apply the algorithm.
                        for( int i=0; i<(int)algorithm.size(); i++ )
                        {
                            auto turnFace = ExportSolverConvert::fromCode("UDFBLR"[algorithm[i]/3]);
                            switch (algorithm[i]%3+1)
                            {
                                case 1:
                                    outMoves.push_back(ColMove(turnFace, true));
                                    break;
                                case 2:
                                    outMoves.push_back(ColMove(turnFace, true));
                                    outMoves.push_back(ColMove(turnFace, true));
                                    break;
                                case 3:
                                    outMoves.push_back(ColMove(turnFace, false));
                                    break;
                            }
                            // outMoves += "UDFBLR"[algorithm[i]/3];
                            // outMoves += ('0' + algorithm[i]%3+1);
                            // cout << "UDFBLR"[algorithm[i]/3] << algorithm[i]%3+1;
                            currentState = ExportSolverConvert::applyMove( algorithm[i], currentState );
                        }
            
                        //--- Jump to the next phase.
                        goto nextPhasePlease;
                    }
        
                    //--- If we've never seen this state (computeId) before, visit it.
                    if( ! newDir )
                    {
                        q.push( newState );
                        newDir = oldDir;
                        lastMove[ newId ] = move;
                        predecessor[ newId ] = oldId;
                    }
                }
            }
        }
        nextPhasePlease:
        ;
    }
    return outMoves;
}

void ExportCubeSolver::computeSolution()
{
    CubeVecInt stateToUse = ExportSolverConvert::cubeToExport(*m_CubeToSolve);

    m_StepSolution.clear();
    auto theSolution = dosolve(stateToUse);
    if (!theSolution.empty())
    {
        m_StepSolution.push_back(theSolution);
    }
    //std::cout << theSolution << std::endl;
}   
