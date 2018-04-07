#include "RubikBase.h"
#include "Cube.h"

#include <Eigen/Dense>

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
    void generateFromMatrix(Eigen::Matrix3f rotMat);
    CubeHandler();
    static void  _generateAllHandler();

    static std::list< CubeHandler > genHandlerList;
    static bool listIsInit;


    std::map< RubikFace, RubikColor> _posToColorMap;
};
