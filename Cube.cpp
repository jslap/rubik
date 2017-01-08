#include "Cube.h"
#include "CubeHandler.h"




Cube::Cube()
{
    edges.push_back(Cubelet<2>(orange, blue));
    edges.push_back(Cubelet<2>(orange, white));
    edges.push_back(Cubelet<2>(orange, green));
    edges.push_back(Cubelet<2>(orange, yellow));
    edges.push_back(Cubelet<2>(red, blue));
    edges.push_back(Cubelet<2>(red, white));
    edges.push_back(Cubelet<2>(red, green));
    edges.push_back(Cubelet<2>(red, yellow));

    edges.push_back(Cubelet<2>(yellow, blue));
    edges.push_back(Cubelet<2>(white, blue));
    edges.push_back(Cubelet<2>(yellow, green));
    edges.push_back(Cubelet<2>(white, green));

    corners.push_back(Cubelet<3>(orange, blue, white));
    corners.push_back(Cubelet<3>(orange, blue, yellow));
    corners.push_back(Cubelet<3>(orange, green, white));
    corners.push_back(Cubelet<3>(orange, green, yellow));

    corners.push_back(Cubelet<3>(red, blue, white));
    corners.push_back(Cubelet<3>(red, blue, yellow));
    corners.push_back(Cubelet<3>(red, green, white));
    corners.push_back(Cubelet<3>(red, green, yellow));
}

void Cube::rotate(const ColMove & move)
{
    rotate(move.first, move.second);
}

void Cube::apply(const ColMoveSeq & moves)
{
    for (const auto& m : moves)
        rotate(m);
}

void Cube::rotate(RubikColor side, bool clockWise)
{
    for (EdgeCube &e: edges)
        e.rotate(side, clockWise);
    for (CornerCube &c: corners)
        c.rotate(side, clockWise);
}

void Cube::makeCanon()
{
    for (EdgeCube &e: edges)
        e.makeCanon();
    std::sort(edges.begin(), edges.end());
    for (CornerCube &c: corners)
        c.makeCanon();
    std::sort(corners.begin(), corners.end());
}

bool Cube::operator==(const Cube & rhs) const
{
    Cube that = *this;
    Cube rhs2 = rhs;
    that.makeCanon();
    rhs2.makeCanon();
    return (that.edges == rhs2.edges) && (that.corners==rhs2.corners);
}


const EdgeCube &Cube::findColorCubePosition(const EdgeCoord & col) const
{
    for (const EdgeCube &e: edges)
        if (isSameCubeColor(e.getColor() , col ))
            return e;
    RASSERT(false, "Cubie not in Cube.");
    return edges[0];
}

const CornerCube &Cube::findColorCubePositionCorner(const CornerCoord & col) const
{
    for (const CornerCube &c: corners)
        if (isSameCubeColor(c.getColor() , col ))
            return c;
    RASSERT(false, "Cubie not in Cube.");
    return corners[0];
}

const EdgeCube &Cube::findPositionCube(const EdgeCoord & col) const
{
    for (const EdgeCube &e: edges)
        if (isSameCubeColor(e.getPosition() , col ))
            return e;
    RASSERT(false, "Cubie not in Cube.");
    return edges[0];
}

const CornerCube &Cube::findPositionCube(const CornerCoord & col) const
{
    for (const CornerCube &c: corners)
        if (isSameCubeColor(c.getPosition() , col ))
            return c;
    RASSERT(false, "Cubie not in Cube.");
    return corners[0];
}


bool Cube::diff(const Cube & rhs, EdgePosList &edgeDiff, CornerPosList &cornerDiff)
{
    edgeDiff.clear();
    cornerDiff.clear();

    Cube that = *this;
    Cube rhs2 = rhs;
    that.makeCanon();
    rhs2.makeCanon();

    EdgeList::iterator eIter = that.edges.begin();
    EdgeList::iterator eIterRhs = rhs2.edges.begin();
    for( ; eIter != that.edges.end(); ++eIter, ++eIterRhs)
        if (eIter->getColor() != eIterRhs->getColor())
            edgeDiff.push_back(eIter->getPosition());

    CornerList::iterator cornIter = that.corners.begin();
    CornerList::iterator cornIterRhs = rhs2.corners.begin();
    for( ; cornIter != that.corners.end(); ++cornIter, ++cornIterRhs)
        if (cornIter->getColor() != cornIterRhs->getColor())
            cornerDiff.push_back(cornIter->getPosition());

    return edgeDiff.empty() && cornerDiff.empty();
}
