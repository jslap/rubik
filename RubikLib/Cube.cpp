#include "Cube.h"

#include "ColorPermMap.h"
#include "CubeHandler.h"

#include <numeric>
#include <range/v3/all.hpp>
#include <set>

namespace
{
    int parityForCorner(const CornerCube& c)
    {
        RubikColor whiteOrYellowPos = c.getPositionIn(white, yellow);
        RubikColor whiteOrYellowColor = c.getColorIn(white, yellow);

        if (c.colorForPosition(whiteOrYellowPos) == whiteOrYellowColor)
        {
            return 0;
        }

        for (RubikColor posToTurn : c.getPosition())
        {
            if (c.getRotate(posToTurn, true).positionForColor(whiteOrYellowColor) == whiteOrYellowPos)
            {
                return 1;
            }
            if (c.getRotate(posToTurn, false).positionForColor(whiteOrYellowColor) == whiteOrYellowPos)
            {
                return 2;
            }
        }
        RASSERT(false, "Should not happen.");
        return 0;
    }

    int parityForEdge(EdgeCube c)
    {
        EdgeCoord edgeColors = c.getColor();
        EdgeCoord edgePos = c.getPosition();

        // Summary: try to bring the edge to its right position by using only Red Blue orange and green (Not whote or yellow)
        // then return 1 if it is flipped, 0 if not fliped.

        // Make at least on color and pos match
        while (!c.hasInPosition(edgeColors[0]) && !c.hasInPosition(edgeColors[1])) // if none match
        {
            c.rotate(edgePos[0], true);
        }
        edgePos = c.getPosition();

        RubikColor goodColPos = c.hasInPosition(edgeColors[0]) ? edgeColors[0] : edgeColors[1];
        while (!isSameCubeColor(edgeColors, edgePos))
        {
            c.rotate(goodColPos, true);
            edgePos = c.getPosition();
        }

        return c.isSolved() ? 0 : 1;
    }

    template <class CubieArrayType>
    int permutationParity(const CubieArrayType& cubies)
    {
        // we do not care about orientation here, so we can sort position and color independantly if we wish.
        // convert each cubie -> pair of coord : first is the color coord sorted, second is the position coord sorted.
        // In terms of permutation, first is the element (x), second is P(x).
        using CubieType = typename CubieArrayType::value_type;
        using CubieCoord = typename CubieType::_MyCubeCoord;
        using PermutationElt = std::pair<CubieCoord, CubieCoord>;
        using PermutationVec = std::vector<PermutationElt>;
        PermutationVec permutationVector;
        for (CubieType c : cubies)
        {
            PermutationElt curElt(c.getColor(), c.getPosition());
            std::sort(curElt.first.begin(), curElt.first.end());
            std::sort(curElt.second.begin(), curElt.second.end());
            permutationVector.push_back(curElt);
        }
        int nbEvenCycle = 0;
        std::set<CubieCoord> treatedCubies;

        auto permFunc = [&permutationVector](const CubieCoord& c) -> CubieCoord {
            auto iter = std::find_if(permutationVector.begin(), permutationVector.end(), [&c](const PermutationElt& p){return p.first == c;});
            if (iter != permutationVector.end())
            {
                return iter->second;
            }

            RASSERT(false, "should not happen");
            return CubieCoord();
        };

        auto getUntreatedElt = [&permutationVector, &treatedCubies] () {
            auto iter = std::find_if(permutationVector.begin(), permutationVector.end(), [&treatedCubies](const PermutationElt& p){return treatedCubies.count(p.first) == 0;});
            if (iter != permutationVector.end())
            {
                return iter->first;
            }

            RASSERT(false, "should not happen");
            return CubieCoord();
        };

        while (treatedCubies.size() != cubies.size())
        {
            std::vector<CubieCoord> curCycle;

            CubieCoord curElt = getUntreatedElt();
            treatedCubies.insert(curElt);
            curCycle.push_back(curElt);
            while (permFunc(curElt) != curCycle.front())
            {
                curElt = permFunc(curElt);

                if (treatedCubies.count(curElt) != 0 )
                {
                    RASSERT(false, "must be an error.");
                    return 0;
                }
                treatedCubies.insert(curElt);
                curCycle.push_back(curElt);
            }
            if (curCycle.size()%2 == 0)
            {
                nbEvenCycle++;
            }
        }
        return (nbEvenCycle%2 == 1) ? -1 : 1;
    }

    bool isRotationOK(const CornerCoord& c1, const CornerCoord& c2)
    {
        if (c1 == c2)
        {
            return true;
        }

        CornerCoord c1Copy = c1;
        std::rotate(c1Copy.begin(), c1Copy.begin()+1, c1Copy.end());
        if (c1Copy == c2)
        {
            return true;
        }

        std::rotate(c1Copy.begin(), c1Copy.begin()+1, c1Copy.end());
        return (c1Copy == c2);
    }

    CornerCube bringToRightPosition(const CornerCube& cubie)
    {
        CornerCube cubieCopy = cubie;
        if (isSameCubeColor( cubieCopy.getPosition(),  cubieCopy.getColor()) )
        {
            return cubieCopy;
        }

        // bring the corner to the right position, regardless of the orientation.
        RubikColor blueOrGreenPos = cubie.getPositionIn(blue, green);

        cubieCopy.rotate(blueOrGreenPos, true);
        if (isSameCubeColor( cubieCopy.getPosition(),  cubieCopy.getColor()))
        {
            return cubieCopy;
        }
        cubieCopy.rotate(blueOrGreenPos, true);
        if (isSameCubeColor( cubieCopy.getPosition(),  cubieCopy.getColor()))
        {
            return cubieCopy;
        }
        cubieCopy.rotate(blueOrGreenPos, true);
        if (isSameCubeColor( cubieCopy.getPosition(),  cubieCopy.getColor()))
        {
            return cubieCopy;
        }

        //rotate the yellow or white.
        RubikColor yellowOrWhite = cubieCopy.getPositionIn(yellow, white);
        cubieCopy.rotate(yellowOrWhite, true);
        cubieCopy.rotate(yellowOrWhite, true);

        cubieCopy.rotate(oppositeColor(blueOrGreenPos), true);
        if (isSameCubeColor( cubieCopy.getPosition(),  cubieCopy.getColor()))
        {
            return cubieCopy;
        }
        cubieCopy.rotate(oppositeColor(blueOrGreenPos), true);
        if (isSameCubeColor( cubieCopy.getPosition(),  cubieCopy.getColor()))
        {
            return cubieCopy;
        }
        cubieCopy.rotate(oppositeColor(blueOrGreenPos), true);
        if (isSameCubeColor( cubieCopy.getPosition(),  cubieCopy.getColor()))
        {
            return cubieCopy;
        }

        RASSERT(false, "Not possible.");
        return cubieCopy;
    }

} // namespace


Cube::Cube():
m_PermMapInst(PermutationMap::getInstance()),
edges({{
        Cubelet<2>(orange, blue),
        Cubelet<2>(orange, white),
        Cubelet<2>(orange, green),
        Cubelet<2>(orange, yellow),
        Cubelet<2>(red, blue),
        Cubelet<2>(red, white),
        Cubelet<2>(red, green),
        Cubelet<2>(red, yellow),

        Cubelet<2>(yellow, blue),
        Cubelet<2>(white, blue),
        Cubelet<2>(yellow, green),
        Cubelet<2>(white, green)}}),
corners({{
        Cubelet<3>(orange, blue, white),
        Cubelet<3>(orange, blue, yellow),
        Cubelet<3>(orange, green, white),
        Cubelet<3>(orange, green, yellow),

        Cubelet<3>(red, blue, white),
        Cubelet<3>(red, blue, yellow),
        Cubelet<3>(red, green, white),
        Cubelet<3>(red, green, yellow)}})
{
}

Cube::Cube(const Cube& c):
    m_PermMapInst(PermutationMap::getInstance()),
    edges(c.edges),
    corners(c.corners)
    {}

const EdgeCube &Cube::getEdge(const EdgeCube& simEdge) const
{
    return getEdge(simEdge.getColor());
}

const EdgeCube &Cube::getEdge(const EdgeCoord& edgeColor) const
{
    return findCubieByColor(edgeColor);
}

const CornerCube &Cube::getCorner(const CornerCube& simCorner) const
{
    return getCorner(simCorner.getColor());
}

const CornerCube &Cube::getCorner(const CornerCoord& cornerColor) const
{
    return findCubieByColor(cornerColor);
}

void Cube::rotate(const ColMove & move, int nbIter)
{
    for (int i = 0; i<nbIter; i++)
    {
        rotate(move);
    }
}

void Cube::rotate(const ColMove & move)
{
    rotate(move.first, move.second);
}

void Cube::apply(const ColMoveSeq & moves)
{
    for (const auto& m : moves)
    {
        rotate(m);
    }
}

template <class CubieType>
void rotateCubieIfInPos(const RubikPerm& permutation, CubieType& cubie, RubikColor side, bool clockWise, int nbIter)
{
    if (cubie.hasInPosition(side))
    {
        cubie.rotate(permutation, side, clockWise, nbIter);
    }
}


void Cube::rotate(RubikColor side, bool clockWise, int nbIter/*=1*/)
{
    const auto& permutationToUse = m_PermMapInst->permutationBySideRotation(side);
    for (auto &e: edges)
    {
        rotateCubieIfInPos(permutationToUse, e, side, clockWise, nbIter);
    }
    for (auto &c: corners)
    {
        rotateCubieIfInPos(permutationToUse, c, side, clockWise, nbIter);
    }
}

bool Cube::isSolved() const
{
    return
        ranges::all_of(edges, &EdgeCube::isSolved) &&
        ranges::all_of(corners, &CornerCube::isSolved);
}

bool Cube::isCrossSolved(RubikColor col) const
{
    return ranges::all_of(edges, [col](const EdgeCube& c) {return !c.hasInColor(col) || c.isSolved();});
}


bool Cube::isValidCube() const
{
    const Cube defCube;

    // 1. must have all the cubes color.
    for (const CornerCube &defCorner: defCube.getCorners())
    {
        if ( std::none_of(getCorners().begin(), getCorners().end(), [&](const CornerCube &c){return isSameCubeColor(defCorner.getColor(), c.getColor());}))
        {
            return false;
        }
    }
    for (const EdgeCube &defEdge: defCube.getEdges())
    {
        if ( std::none_of(getEdges().begin(), getEdges().end(), [&](const EdgeCube &c){return isSameCubeColor(defEdge.getColor(), c.getColor());}))
        {
            return false;
        }
    }

    // 2. must have all the cubes position.
    for (const CornerCube &defCorner: defCube.getCorners())
    {
        if ( std::none_of(getCorners().begin(), getCorners().end(), [&](const CornerCube &c){return isSameCubeColor(defCorner.getPosition(), c.getPosition());}))
        {
            return false;
        }
    }
    for (const EdgeCube &defEdge: defCube.getEdges())
    {
        if ( std::none_of(getEdges().begin(), getEdges().end(), [&](const EdgeCube &c){return isSameCubeColor(defEdge.getPosition(), c.getPosition());}))
        {
            return false;
        }
    }

    // 3. must have 8 corners and 12 edges.
    if (getCorners().size() != defCube.getCorners().size() ||
        getEdges().size() != defCube.getEdges().size())
    {
        return false;
    }

    // 3.1 corner cubes rotation should not be reversed compare to solved cube.
    for (const CornerCube &c: getCorners())
    {
        CornerCube curCubieatRightPos = bringToRightPosition(c);

        if (!isRotationOK(curCubieatRightPos.getPosition(), curCubieatRightPos.getColor()))
        {
            return false;
        }
    }


    // 4. must be solvable.
    // a) check corner orientation parity.
    int cornerParitySum = ranges::accumulate(corners | ranges::view::transform(&parityForCorner), 0 );
    if (cornerParitySum%3 != 0)
    {
        return false;
    }

    // b) check the edge orientation parity.
    int edgeParitySum = ranges::accumulate(edges | ranges::view::transform(&parityForEdge), 0 );
    if (edgeParitySum%2 != 0)
    {
        return false;
    }

    // c) check the permutation parity.
    int cornerPermutationParity = permutationParity(corners);
    int edgePermutationParity = permutationParity(edges);
    return (cornerPermutationParity*edgePermutationParity >= 0);
}

bool Cube::operator==(const Cube & rhs) const
{
    return (edges == rhs.edges) && (corners==rhs.corners);
}

void Cube::rotateSinglePiece(const CornerCoord & col, bool cw)
{
    _findCubieByColor<CornerCube>(col).rotateSinglePiece(cw);
}

void Cube::swapSinglePiece(const CornerCoord & col)
{
    _findCubieByColor<CornerCube>(col).swapSinglePiece(col[0], col[1]);
}

void Cube::flipEdge(const EdgeCoord& col)
{
    _findCubieByColor<EdgeCube>(col).rotateSinglePiece(true);
}

void Cube::swapEdges(const EdgeCoord& col1, const EdgeCoord& col2)
{
    auto &col1Cubie = _findCubieByColor<EdgeCube>(col1);
    auto &col2Cubie = _findCubieByColor<EdgeCube>(col2);
    col1Cubie.swapPositionWith(col2Cubie);
}

void Cube::swapCorners(const CornerCoord& col1, const CornerCoord& col2)
{
    auto &col1Cubie = _findCubieByColor<CornerCube>(col1);
    auto &col2Cubie = _findCubieByColor<CornerCube>(col2);
    col1Cubie.swapPositionWith(col2Cubie);
}


bool Cube::diff(const Cube & rhs, EdgePosList &edgeDiff, CornerPosList &cornerDiff)
{
    edgeDiff.clear();
    cornerDiff.clear();

    Cube that = *this;
    Cube rhs2 = rhs;

    auto eIter = that.edges.begin();
    auto eIterRhs = rhs2.edges.begin();
    for( ; eIter != that.edges.end(); ++eIter, ++eIterRhs)
    {
        if (eIter->getColor() != eIterRhs->getColor())
        {
            edgeDiff.push_back(eIter->getPosition());
        }
    }

    auto cornIter = that.corners.begin();
    auto cornIterRhs = rhs2.corners.begin();
    for( ; cornIter != that.corners.end(); ++cornIter, ++cornIterRhs)
    {
        if (cornIter->getColor() != cornIterRhs->getColor())
        {
            cornerDiff.push_back(cornIter->getPosition());
        }
    }

    return edgeDiff.empty() && cornerDiff.empty();
}
