#include "RubikBase.h"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>


const std::vector< RubikColor > RubikBase::RubikColors({white, green, red, blue, orange, yellow});
const std::vector<std::pair< RubikColor, Vector3i >> RubikBase::ColorVecPairs({
    std::make_pair(white,   Vector3i(0,     1,  0)),
    std::make_pair(yellow,  Vector3i(0,    -1,  0)),
    std::make_pair(red,     Vector3i(0,     0,  1)),
    std::make_pair(orange,  Vector3i(0,     0,  -1)),
    std::make_pair(green,   Vector3i(1,     0,  0)),
    std::make_pair(blue,    Vector3i(-1,     0,  0))
});

const std::map < RubikColor , Vector3i > RubikBase::colorVecMap({
    std::begin(RubikBase::ColorVecPairs),
    std::end(RubikBase::ColorVecPairs)});

namespace
{
    using ColorNameBimap = boost::bimap<RubikColor, std::string>;
    ColorNameBimap colorNameBimap = boost::assign::list_of< ColorNameBimap::relation >
    ( white,    "white")
    ( green,    "green")
    ( blue,     "blue")
    ( yellow,   "yellow")
    ( orange,   "orange")
    ( red,      "red")
    ( noColor,  "no Color");
} //namespace

std::string colorName( RubikColor col)
{
    auto iter = colorNameBimap.left.find(col);
    if (iter != colorNameBimap.left.end())
    {
        return iter->second;
    }

    RASSERT(false, "undefined color.");
    return "error";
}

std::string orientationName( RubikOrientation o)
{
    switch(o)
    {
        case WellOriented:
            return "WellOrient";
        case Twist1:
            return "Twist1";
        case Twist2:
            return "Twist2";
    }
    return "noOrientation";
}

RubikColor colorFromName( const std::string& s)
{
    auto iter = colorNameBimap.right.find(s);
    if (iter != colorNameBimap.right.end())
    {
        return iter->second;
    }

    RASSERT(false, "undefined color.");
    return noColor;
}


Vector3i getVectorFromColor(RubikColor col)
{
    return RubikBase::colorVecMap.at(col);
}


RubikColor getColorFromVecf(const Vector3f &vec)
{
    return getColorFromVeci(roundVec(vec));
}
RubikColor getColorFromVeci(const Vector3i &vec)
{
    auto foundIter = std::find_if(RubikBase::colorVecMap.begin(), RubikBase::colorVecMap.end(),
        [&vec] (const decltype(RubikBase::colorVecMap)::value_type& cv ) {
            return cv.second == vec;
        });
    return (foundIter != RubikBase::colorVecMap.end()) ? foundIter->first : noColor;
}


Vector3i roundVec(const Vector3f & rhs)
{
    Vector3i ret(
                 round(rhs.x()),
                 round(rhs.y()),
                 round(rhs.z())
                 );
    return ret;
}
