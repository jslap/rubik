#include "RubikBase.h"

#include <Eigen/Dense>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

const std::vector<std::pair< RubikColor, Vector3i >> & RubikBase::ColorVecPairs()
{ 
    static std::vector<std::pair< RubikColor, Vector3i >> _ColorVecPairs({
        std::make_pair(white,   Vector3i(0,     1,  0)),
        std::make_pair(yellow,  Vector3i(0,    -1,  0)),
        std::make_pair(red,     Vector3i(0,     0,  1)),
        std::make_pair(orange,  Vector3i(0,     0,  -1)),
        std::make_pair(green,   Vector3i(1,     0,  0)),
        std::make_pair(blue,    Vector3i(-1,    0,  0))
    });
    return _ColorVecPairs;
} // LCOV_EXCL_LINE

const RubikBase::ColorVecMap& RubikBase::colorVecMap()
{
    static ColorVecMap _colorVecMap({
        std::begin(RubikBase::ColorVecPairs()),
        std::end(RubikBase::ColorVecPairs())});
    return _colorVecMap;
} // LCOV_EXCL_LINE

namespace
{
    using ColorNameBimap = boost::bimap<RubikColor, std::string>;
    const ColorNameBimap& colorNameBimap()
    {
        static ColorNameBimap _colorNameBimap = boost::assign::list_of< ColorNameBimap::relation >
            ( white,    "white")
            ( green,    "green")
            ( blue,     "blue")
            ( yellow,   "yellow")
            ( orange,   "orange")
            ( red,      "red")
            ( noColor,  "no Color");
        return _colorNameBimap;
    } // LCOV_EXCL_LINE

    Vector3i roundVec(const Vector3f & rhs)
    {
        return Vector3i (
                    round(rhs.x()),
                    round(rhs.y()),
                    round(rhs.z())
                    );
    }

} //namespace

std::string colorName( RubikColor col)
{
    auto iter = colorNameBimap().left.find(col);
    RASSERT(iter != colorNameBimap().left.end(), "undefined color.");
    return iter->second;
}

std::string orientationName( RubikOrientation o)
{
    switch(o)
    {
        case WellOriented:
            return "WellOriented";
        case Twist1:
            return "Twist1";
        case Twist2:
            return "Twist2";
    }
    RASSERT(false, "undefined orientation.");
}

RubikColor colorFromName( const std::string& s)
{
    auto iter = colorNameBimap().right.find(s);
    RASSERT(iter != colorNameBimap().right.end(), "undefined color.");
    return iter->second;
}


Vector3i getVectorFromColor(RubikColor col)
{
    return RubikBase::colorVecMap().at(col);
}


RubikColor getColorFromVecf(const Vector3f &vec)
{
    return getColorFromVeci(roundVec(vec));
}
RubikColor getColorFromVeci(const Vector3i &vec)
{
    auto foundIter = std::find_if(RubikBase::colorVecMap().begin(), RubikBase::colorVecMap().end(),
        [&vec] (const RubikBase::ColorVecMap::value_type& cv ) {
            return cv.second == vec;
        });
    return (foundIter != RubikBase::colorVecMap().end()) ? foundIter->first : noColor;
}

Vector3i intCrossProduct(const Vector3i& v1, const Vector3i& v2)
{
    Vector3f crossProd = v1.cast<float>().cross(v2.cast<float>());
    return roundVec(crossProd);
}

Vector3i colorVectorCroosProd(RubikColor col1, RubikColor col2)
{
    Vector3i col1Vec = getVectorFromColor(col1);
    Vector3i col2Vec = getVectorFromColor(col2);
    return intCrossProduct(col1Vec, col2Vec);
}
