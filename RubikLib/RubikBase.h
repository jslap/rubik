#pragma once

#include "RubikAssert.h"

#include <vector>
#include <list>
#include <map>
#include <range/v3/all.hpp>
#include <Eigen/Core>
#include <iostream>
#include <array>

using Eigen::Vector3i;
using Eigen::Vector3f;

// Do not change the order.
enum RubikColor  : unsigned char {white = 0, green, red ,  yellow, blue , orange, noColor};
enum RubikFace   : unsigned char {Up    = 0, Right, Front, Down,   Left,  Back,  };
enum RubikOrientation : unsigned char {WellOriented = 0, Twist1, Twist2};

class RubikBase {
public:
    static constexpr std::size_t NumberColor = 6;
    static constexpr std::array< RubikColor, NumberColor > RubikColors() { return {{white, green, red, blue, orange, yellow}}; }

    static const std::vector<std::pair< RubikColor, Vector3i >>& ColorVecPairs();
    using ColorVecMap = std::map < RubikColor , Vector3i >;
    static const ColorVecMap& colorVecMap();
};

inline RubikColor oppositeColor(RubikColor c) { return RubikColor((c + 3) % 6);}

std::string colorName( RubikColor col);
RubikColor colorFromName( const std::string& s);
inline std::ostream& operator<<(std::ostream & os, RubikColor c) {os << "col(" << colorName(c) << ")"; return os;}
std::string orientationName( RubikOrientation o);
inline std::ostream& operator<<(std::ostream & os,  RubikOrientation o) {os << orientationName(o); return os;}

namespace cereal
{
  template <class Archive> inline
  std::string save_minimal( Archive const &, RubikColor const & t )
  {
    return colorName( t );
  }

  template <class Archive> inline
  void load_minimal( Archive const &, RubikColor & t, std::string const & value )
  {
    t = colorFromName( value );
  }
}

Vector3i getVectorFromColor(RubikColor col);

Vector3i colorVectorCroosProd(RubikColor col1, RubikColor col2);
Vector3i intCrossProduct(const Vector3i& v1, const Vector3i& v2);
RubikColor getColorFromVeci(const Vector3i &vec);
RubikColor getColorFromVecf(const Vector3f &vec);

template <std::size_t cubeDim>
using CubeCoord = std::array<RubikColor, cubeDim>;


typedef CubeCoord< 2 >  EdgeCoord;
typedef CubeCoord< 3 >  CornerCoord;

template <std::size_t cubeDim>
CubeCoord<cubeDim> getInvalidCoord()
{
    CubeCoord<cubeDim> c;
    c.fill(noColor);
    return c;
}

template <std::size_t cubeDim>
Vector3i getVectorFromCoord(CubeCoord< cubeDim > coord)
{
    return ranges::accumulate( (coord | ranges::view::transform(&getVectorFromColor)) , Vector3i(Vector3i::Zero()));
}

typedef std::vector< EdgeCoord > EdgePosList;
typedef std::vector< CornerCoord > CornerPosList;

typedef std::pair< RubikFace , bool > PosMove;
typedef std::pair< RubikColor , bool > ColMove;
typedef std::list< PosMove > PosMoveSeq;
typedef std::list< ColMove > ColMoveSeq;

inline std::ostream& operator<<(std::ostream & os, ColMoveSeq seq) 
{
    for (auto& m: seq)
        os << "mov(" << m.first << ", " << m.second << "), "; 
    return os;
}


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
