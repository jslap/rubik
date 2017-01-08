#pragma once

#include <vector>
#include <list>
#include <map>
#include <Eigen/Dense>
#include <boost/array.hpp>

void AsertFunc(const std::string & reason );
#define RASSERT(pred, str) if (!(pred)) {AsertFunc(str);}

using Eigen::Vector3i;
using Eigen::Vector3f;

enum RubikColor {white = 0, green, red , blue ,orange,    yellow, noColor};
enum RubikFace  {Up    = 0, Right, Front,   Left, Back ,  Down};

class RubikBase {
public:
    static const std::vector< RubikColor > RubikColors;
    static const std::vector<std::pair< RubikColor, Vector3i >> ColorVecPairs;
    static const std::map < RubikColor , Vector3i > colorVecMap;
};

std::string colorName( RubikColor col);
Vector3i getVectorFromColor(RubikColor col);

Vector3i roundVec(const Vector3f & rhs);
RubikColor getColorFromVeci(const Vector3i &vec);
RubikColor getColorFromVecf(const Vector3f &vec);

template <int cubeDim>
using CubeCoord = boost::array<RubikColor, cubeDim>;


typedef CubeCoord< 2 >  EdgeCoord;
typedef CubeCoord< 3 >  CornerCoord;

template <int cubeDim>
Vector3i getVectorFromCoord(CubeCoord< cubeDim > coord)
{
    Vector3i retVal = Vector3i::Zero();
    for (int i = 0; i<cubeDim; i++)
        retVal += getVectorFromColor(coord[i]);
    return retVal;
}

typedef std::vector< EdgeCoord > EdgePosList;
typedef std::vector< CornerCoord > CornerPosList;

typedef std::pair< RubikFace , bool > PosMove;
typedef std::pair< RubikColor , bool > ColMove;
typedef std::list< PosMove > PosMoveSeq;
typedef std::list< ColMove > ColMoveSeq;

template < class Coord>
bool isSameCubeColor(const Coord& c1, const Coord &c2);

template <>
inline bool isSameCubeColor< EdgeCoord >(const EdgeCoord& c1, const EdgeCoord &c2)
{
    return (c1[0] == c2[0] || c1[0] == c2[1]) &&
        (c1[1] == c2[0] || c1[1] == c2[1]);
}

template <>
inline bool isSameCubeColor< CornerCoord >(const CornerCoord& c1, const CornerCoord &c2)
{
    return
        (c1[0] == c2[0] || c1[0] == c2[1] || c1[0] == c2[2]) &&
        (c1[1] == c2[0] || c1[1] == c2[1] || c1[1] == c2[2]) &&
        (c1[2] == c2[0] || c1[2] == c2[1] || c1[2] == c2[2])
    ;
}
