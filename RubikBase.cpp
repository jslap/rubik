#include "RubikBase.h"

void AsertFunc(const std::string & reason )
{
    printf("Assert failed %s\n", reason.c_str());
    assert(false);
}

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

std::string colorName( RubikColor col)
{
    switch (col )
    {
        case white:
            return "white";
        case green:
            return "green";
        case blue:
            return "blue";
        case yellow:
            return "yellow";
        case orange:
            return "orange";
        case red:
            return "red";
        case noColor:
            return "noColor";
    }
    return "error";
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
    for (auto& kv : RubikBase::colorVecMap)
    {
        if (kv.second == vec)
            return kv.first;
    }
    return noColor;
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
