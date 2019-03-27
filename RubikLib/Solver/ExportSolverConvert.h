#pragma once

#include "Cubelet.h"
#include "RubikBase.h"

#include <vector>
#include <string>

class Cube;

namespace ExportSolverConvert
{

typedef std::vector<int> CubeVecInt;

CubeVecInt cubeToExport(const Cube& c);

CubeVecInt applyMove ( int move, CubeVecInt state ) ;

std::string::value_type colorToCode(RubikColor c);
RubikColor fromCode(std::string::value_type c);

const EdgeCube& getEdgeByStrCode(const Cube& cube, const std::string& s);

const CornerCube& getCornerByStrCode(const Cube& cube, const std::string& s);

template <class CubieType>
std::string getStrCodeForColor(const CubieType& c);


}