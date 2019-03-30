#pragma once

#include "Cubelet.h"
#include "RubikBase.h"

#include <vector>
#include <string>

class Cube;

namespace ExportSolverConvert
{

using CubeVecInt = std::vector<int>;

CubeVecInt cubeToExport(const Cube& c);
Cube cubeFromExport(const CubeVecInt& c);

CubeVecInt applyMove ( int move, CubeVecInt state ) ;

std::string::value_type colorToCode(RubikColor c);
RubikColor colorFromCode(std::string::value_type c);

const EdgeCube& getEdgeByStrCode(const Cube& cube, const std::string& s);

const CornerCube& getCornerByStrCode(const Cube& cube, const std::string& s);

template <class CubieType>
std::string getStrCodeForColor(const CubieType& c);


}