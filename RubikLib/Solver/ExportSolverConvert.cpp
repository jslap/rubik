#include "ExportSolverConvert.h"

#include "RubikBase.h"
#include "Cube.h"

using namespace ExportSolverConvert;

namespace
{
constexpr int affectedCubies[][8] = {
  {  0,  1,  2,  3,  0,  1,  2,  3 },   // U 
  {  4,  7,  6,  5,  4,  5,  6,  7 },   // D 
  {  0,  9,  4,  8,  0,  3,  5,  4 },   // F 
  {  2, 10,  6, 11,  2,  1,  7,  6 },   // B 
  {  3, 11,  7,  9,  3,  2,  6,  5 },   // L 
  {  1,  8,  5, 10,  1,  0,  4,  7 },   // R 
};
//--- Define the goal.
const std::string goal[] = { "UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL", "BR", "BL",
        "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR" };

} // namespace

Cube ExportSolverConvert::cubeFromExport(const CubeVecInt& c)
{
    Cube retVal;
    for( int edgeInd=0; edgeInd<12; edgeInd++ )
    {
        auto curEdgePos = goal[edgeInd];
        auto curEdgeCol = goal[c[edgeInd]];
        EdgeCube::_MyCubeCoord coordPos = {colorFromCode(curEdgePos[0]), colorFromCode(curEdgePos[1])}; 
        EdgeCube::_MyCubeCoord coordCol = {colorFromCode(curEdgeCol[0]), colorFromCode(curEdgeCol[1])}; 
        auto oldEdge = retVal.findCubieByColor(coordCol);
        while (oldEdge.getColor() != coordCol)
        {
            std::rotate(coordPos.begin(), coordPos.begin()+1, coordPos.end());
            std::rotate(coordCol.begin(), coordCol.begin()+1, coordCol.end());
        }
        for (int i = 0; i < c[edgeInd + 20]; i++)
        {
            std::rotate(coordPos.begin(), coordPos.begin()+1, coordPos.end());
        }
        EdgeCube curCube{coordPos, coordCol, static_cast<RubikOrientation>(c[edgeInd + 20])};
        retVal.setEdge(curCube);
    }
    for( int cornerInd=0; cornerInd<8; cornerInd++ )
    {
        auto curCornerPos = goal[cornerInd+12];
        auto curCornerCol = goal[c[cornerInd+12]];
        CornerCube::_MyCubeCoord coordPos = {colorFromCode(curCornerPos[0]), colorFromCode(curCornerPos[1]), colorFromCode(curCornerPos[2])}; 
        CornerCube::_MyCubeCoord coordCol = {colorFromCode(curCornerCol[0]), colorFromCode(curCornerCol[1]), colorFromCode(curCornerCol[2])}; 
        auto oldCorner = retVal.findCubieByColor(coordCol);
        for (int i = 0; i < 3 ; i++)
        {
            auto curColorToFind = oldCorner.getColor()[i];
            auto foundIter = std::find(coordCol.begin(), coordCol.end(), curColorToFind);
            auto foundPos = std::distance(coordCol.begin(), foundIter);
            if (i != foundPos)
            {
                std::swap(coordCol[i], coordCol[foundPos]);
                std::swap(coordPos[i], coordPos[foundPos]);
            }
        }

        for (int i = 0; i < c[cornerInd+12 + 20]; i++)
        {
            std::rotate(coordPos.begin(), coordPos.begin()+2, coordPos.end());
        }
        CornerCube curCube{coordPos, coordCol, static_cast<RubikOrientation>(c[cornerInd+12 + 20])};
        retVal.setCorner(curCube);
    }

    return retVal;
}



CubeVecInt ExportSolverConvert::cubeToExport(const Cube& cubeToConvert)
{
    CubeVecInt stateToUse( 40 );
    for( int i=0; i<20; i++ )
    {
        auto curGoalCubie = goal[i];
        std::string curCubieColor;
        if (curGoalCubie.length() == 2)
        {
            auto curEdge = ExportSolverConvert::getEdgeByStrCode(cubeToConvert, curGoalCubie);
            curCubieColor = ExportSolverConvert::getStrCodeForColor(curEdge);
            for (int permIndex = 0 ; permIndex < 20 ;  permIndex++)
            {
                auto curPermElem = goal[permIndex];
                if (curPermElem.length() != 2)
                    continue;
                if (isSameCubeColor(curEdge.getColor(), {ExportSolverConvert::colorFromCode(curPermElem[0]), ExportSolverConvert::colorFromCode(curPermElem[1])}))
                {
                    stateToUse[i] = permIndex;
                    break;
                }
            }
            stateToUse[i+20] = curEdge.getOrientation();
        }
        else
        {
            auto curCorner = ExportSolverConvert::getCornerByStrCode(cubeToConvert, curGoalCubie);
            curCubieColor = ExportSolverConvert::getStrCodeForColor(curCorner);
            for (int permIndex = 0 ; permIndex < 20 ;  permIndex++)
            {
                auto curPermElem = goal[permIndex];
                if (curPermElem.length() != 3)
                    continue;
                if (isSameCubeColor(curCorner.getColor(), {
                        ExportSolverConvert::colorFromCode(curPermElem[0]), 
                        ExportSolverConvert::colorFromCode(curPermElem[1]), 
                        ExportSolverConvert::colorFromCode(curPermElem[2])}))
                {
                    stateToUse[i] = permIndex;
                    break;
                }
            }
            stateToUse[i+20] = curCorner.getOrientation();
        }
    }
    return stateToUse;
}


CubeVecInt ExportSolverConvert::applyMove ( int move, CubeVecInt state ) 
{
    int turns = move % 3 + 1;
    int face = move / 3;
    while( turns-- )
    {
        CubeVecInt oldState = state;
        for( int i=0; i<8; i++ )
        {
            int isCorner = i > 3;
            int target = affectedCubies[face][i] + isCorner*12;
            int killer = affectedCubies[face][(i&3)==3 ? i-3 : i+1] + isCorner*12;;
            int orientationDelta = (i<4) ? (face>1 && face<4) : (face<2) ? 0 : 2 - (i&1);
            state[target] = oldState[killer];
            state[target+20] = oldState[killer+20] + orientationDelta;
            if( !turns )
            {
                state[target+20] %= 2 + isCorner;
            }
        }
    }
    return state;
}

// #1
// U green    
// D blue
// F white
// B yellow
// L orange
// R red

// #2
// U green    
// D blue
// F yellow
// B white
// L red
// R orange


//#3
// U blue    
// D green
// F yellow
// B white
// L orange
// R red

//#4
// U blue    
// D green
// F white
// B yellow
// L red
// R orange
std::string::value_type ExportSolverConvert::colorToCode(RubikColor c)
{
    switch (c)
    {
        case white: return 'B';
        case yellow: return 'F';
        case blue: return 'D';
        case green: return 'U';
        case orange: return 'R';
        case red: return 'L';
        default: Throw("No such color.");
    }

}
// #2
// U green    
// D blue
// F yellow
// B white
// L red
// R orange

RubikColor ExportSolverConvert::colorFromCode(std::string::value_type c)
{
    switch (c)
    {
        case 'U': return green;
        case 'D': return blue;
        case 'R': return orange;
        case 'L': return red;
        case 'F': return yellow;
        case 'B': return white;
    }
    Throw("No such color.");
}

// enum class ThistleMoves
// {
//     //"UDFBLR"
//     Green1 = 0, Green2 = 1, Green3 = 2// "U"
//     Blue1 = 3+0, Blue2 = 3+1, Blue3 = 3+2, // "D"
//     Yellow1 = 6+0, Yellow2 = 6+1, Yellow3 = 6+2, // "F"
//     White1 = 9+0, White2 = 9+1, White3 = 9+2, // "B"
//     Red1 = 12+0, Red2 = 12+1, Red3 = 12+2, // "L"
//     Orange1 = 15+0, Orange2 = 15+1, Orange3 = 15+2, // "R"
// };


const EdgeCube& ExportSolverConvert::getEdgeByStrCode(const Cube& cube, const std::string& s)
{
    EdgeCube::_MyCubeCoord coord = {colorFromCode(s[0]), colorFromCode(s[1])}; 
    return cube.findCubieByPosition(coord);
}

const CornerCube& ExportSolverConvert::getCornerByStrCode(const Cube& cube, const std::string& s)
{
    CornerCube::_MyCubeCoord coord = {colorFromCode(s[0]), colorFromCode(s[1]), colorFromCode(s[2])}; 
    return cube.findCubieByPosition(coord);
}

template <class CubieType>
std::string ExportSolverConvert::getStrCodeForColor(const CubieType& c)
{
    std::string retVal;
    for (auto col : c.getColor())
    {
        retVal += colorToCode(col);
    }
    return retVal;
}

template
std::string ExportSolverConvert::getStrCodeForColor<EdgeCube>(const EdgeCube& c);
template
std::string ExportSolverConvert::getStrCodeForColor<CornerCube>(const CornerCube& c);
