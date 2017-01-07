//
//  RubikUtil.h
//  RubikTest
//
//  Created by Jean-Simon Lapointe on 2013-01-13.
//  Copyright (c) 2013 Jean-Simon Lapointe. All rights reserved.
//

#ifndef __RubikTest__RubikUtil__
#define __RubikTest__RubikUtil__

#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/array.hpp>
#include <map>
#include <deque>
#include <list>

#include <Eigen/Dense>
using namespace Eigen;


#import <Foundation/Foundation.h>

void AsertFunc(const std::string & reason );
#define RASSERT(pred, str) if (!(pred)) {AsertFunc(str);}


enum RubikColor {white = 0, green, red , blue ,orange,    yellow, noColor};
const std::vector< RubikColor > &RubikColors();
enum RubikFace  {Up    = 0, Right, Front,   Left, Back ,  Down};

std::string colorName( RubikColor col);
Vector3i roundVec(const Vector3f & rhs);
RubikColor getColorFromVeci(const Vector3i &vec);
RubikColor getColorFromVecf(const Vector3f &vec);

Vector3i getVectorFromColor(RubikColor col);


void initRubik();

class RubikPerm
{
public:
    typedef std::map< RubikColor, RubikColor > Func;

    RubikPerm()
    {
        forward = inverse = _defaultFunc();
    }
    RubikPerm(const RubikPerm& rhs)
    {
        forward = rhs.forward;
        inverse = rhs.inverse;
    }

    RubikPerm& operator=(const RubikPerm& rhs)
    {
        if (this != &rhs)
        {
            forward = rhs.forward;
            inverse = rhs.inverse;
        }
        return *this;
    }

    void setForward(Func forwardFunc)
    {
        forward = forwardFunc;
        inverse = _computeInverse(forwardFunc);
    }
    
    RubikColor nextColor(RubikColor col)
    {
        return forward[col];
    }
    
    RubikColor prevColor(RubikColor col)
    {
        return inverse[col];
    }
    
    
private:
    Func _defaultFunc() const
    {
        Func ret;
        for (auto i : RubikColors())
            ret[i] = i;
        return ret;
    }
    
    Func _computeInverse(Func forwardFunc) const
    {
        Func ret;
        for (auto i : RubikColors())
            ret[forwardFunc[i]] = i;
        return ret;
    }
    
    Func forward;
    Func inverse;
};




template <int cubeDim>
using CubeCoord = boost::array<RubikColor, cubeDim>;

template <int cubeDim>
Vector3i getVectorFromCoord(CubeCoord< cubeDim > coord)
{
    Vector3i retVal = Vector3i::Zero();
    for (int i = 0; i<cubeDim; i++)
        retVal += getVectorFromColor(coord[i]);
    return retVal;
}


template <int cubeDim>
class Cubelet
{
public:
    Cubelet(const CubeCoord< cubeDim >& pos)
    {
        position = pos;
        color = pos;
    }
    Cubelet(RubikColor p0, RubikColor p1) // starting pos
    {
        if (cubeDim != 2)
            NSLog(@"cubeDim != 2");
        
        position[0] = color[0] = p0;
        position[1] = color[1] = p1;
    }
    Cubelet(RubikColor p0, RubikColor p1, RubikColor p2) // starting pos
    {
        if (cubeDim != 3)
            NSLog(@"cubeDim != 3");
        
        position[0] = color[0] = p0;
        position[1] = color[1] = p1;
        position[2] = color[2] = p2;
    }
    
    const CubeCoord< cubeDim > &getPosition() const {return position;};
    const CubeCoord< cubeDim > &getColor() const {return color;}
    
    RubikColor positionForColor(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (color[i] == col)
                return position[i];
        RASSERT(false, "Color not in cubie.");
        return noColor;
    }

    RubikColor colorForPosition(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (position[i] == col)
                return color[i];
        RASSERT(false, "Color not in cubie.");
        return noColor;
    }

    RubikColor getColorNot(RubikColor notThisColor) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (color[i] != notThisColor)
                return color[i];
        RASSERT(false, "Color is in all cubie pos..");
        return noColor;
    }
    
    RubikColor getPositionNot(RubikColor notThisPos) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (position[i] != notThisPos)
                return position[i];
        RASSERT(false, "Color is in all cubie pos..");
        return noColor;
    }
    
    bool hasInPosition(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (position[i] == col)
                return true;
        return false;
    }
    
    bool hasInColor(RubikColor col) const
    {
        for (int i = 0; i<cubeDim; i++)
            if (color[i] == col)
                return true;
        return false;
    }
    
    
    void rotate(RubikColor side, bool clockWise);
    
    void makeCanon();
    
    bool operator !=(const  Cubelet<cubeDim>& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator ==(const  Cubelet<cubeDim>& rhs) const
    {
        return position==rhs.position && color==rhs.color;
    }
    
    bool operator< (const  Cubelet<cubeDim>& rhs) const
    {
        return std::lexicographical_compare(position.begin(), position.end(), rhs.position.begin(), rhs.position.end());
    }
private:
    
    CubeCoord< cubeDim > position;
    CubeCoord< cubeDim > color;
};


typedef Cubelet< 2 >  EdgeCube;
typedef Cubelet< 3 >  CornerCube;

typedef std::vector< EdgeCube > EdgeList;
typedef std::vector< CornerCube > CornerList;

typedef CubeCoord< 2 >  EdgeCoord;
typedef CubeCoord< 3 >  CornerCoord;

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

class CubeHandler;

class Cube
{
public:
    Cube();
    
    const EdgeList &getEdges() const {return edges;};
    const CornerList &getCorners() const {return corners;}

    
    void rotate(const ColMove & move)
    {
        rotate(move.first, move.second);
    }
    void rotate(RubikColor side, bool clockWise)
    {
        for (EdgeCube &e: edges)
            e.rotate(side, clockWise);
        for (CornerCube &c: corners)
            c.rotate(side, clockWise);
    }
    
    
    void makeCanon();
    
    bool operator==(const Cube & rhs) const
    {
        Cube that = *this;
        Cube rhs2 = rhs;
        that.makeCanon();
        rhs2.makeCanon();
        return (that.edges == rhs2.edges) && (that.corners==rhs2.corners);
    }
    
    bool diff(const Cube & rhs, EdgePosList &edgeDiff, CornerPosList &cornerDiff);
    
    const EdgeCube & findColorCubePosition(const EdgeCoord & col) const;
    const CornerCube &findColorCubePositionCorner(const CornerCoord & col) const;

    const EdgeCube &findPositionCube(const EdgeCoord & col) const;
    const CornerCube &findPositionCube(const CornerCoord & col) const;

    ColMoveSeq solve(int step) ;
    
    bool isThistlethwaiteStep1_OK() const;
    
    
private:
    ColMoveSeq _solveSingleStep(int step) ;

    ColMoveSeq _solveStepCross() ;
    ColMoveSeq _solveStepCrossElt(RubikColor col);
    
    ColMoveSeq _solveStepWhiteLayer() ;
    ColMoveSeq _solveStepWhiteLayerElt(const CornerCoord& piece);
    
    ColMoveSeq _solveStepMiddleLayer() ;
    ColMoveSeq _solveStepMiddleLayerElt(const EdgeCoord& piece);

    ColMoveSeq _solveStepTopCross() ;

    ColMoveSeq _solveStepTopCorners() ;

    ColMoveSeq _solveStepTopCornersPos() ;

    ColMoveSeq _solveStepTopEdges() ;

    void _addAndApply(ColMove aMove, ColMoveSeq &seq);
    void _addAndApply(const CubeHandler& handler, PosMove aMove, ColMoveSeq &seq);

    
    static const int NbEdge = 12;
    static const int NbCorner = 8;
    
    EdgeList edges;
    CornerList corners;
};

class CubeHandler
{
public:
    static CubeHandler fromTopFront(RubikColor topFace, RubikColor frontFace);
    static CubeHandler fromTopBack(RubikColor topFace, RubikColor backFace);
    static CubeHandler fromTopRight(RubikColor topFace, RubikColor rightFace);
    static CubeHandler fromTopLeft(RubikColor topFace, RubikColor leftFace);

    static CubeHandler genHandler(CornerCube cubie, RubikFace face1, RubikFace face2, RubikFace face3);
    static CubeHandler genHandler(EdgeCube cubie, RubikFace face1, RubikFace face2, bool whiteOnTop);
    
    void rotate(Cube &cube, RubikFace face, bool cw) const;
    CornerCube rotate(const CornerCube &cubie, RubikFace face, bool cw) const
    {
        CornerCube cubieCopy = cubie;
        cubieCopy.rotate(_getCol(face), cw);
        return cubieCopy;
    }
    
    const EdgeCube& getCubie(const Cube &cube, RubikFace face1, RubikFace face2);
    const CornerCube& getCubie(const Cube &cube, RubikFace face1, RubikFace face2, RubikFace face3);
    
    
    ColMoveSeq convertSeq(const PosMoveSeq & posSeq) const;
    ColMove convertMove(PosMove posMove) const;
    
    static PosMoveSeq genSeq();
    
    RubikColor _getCol(RubikFace) const;
private:
    void generateFromMatrix(Matrix3f rotMat);
    CubeHandler();
    static void  _generateAllHandler();

    static std::list< CubeHandler > genHandlerList;
    static bool listIsInit;

    
    std::map< RubikFace, RubikColor> _posToColorMap;
};


#endif /* defined(__RubikTest__RubikUtil__) */
