#include <Eigen/Dense>

#include "ColorPermMap.h"

using Eigen::Vector3f;

PermutationMap::PermutationMap()
{
    m_PermutationBySideRotation.resize(RubikBase::RubikColors.size());
    for (auto rotCol : RubikBase::RubikColors)
    {
        RubikPerm::Func forwFunc;
        auto ax  = Eigen::AngleAxis<float>(-3.1416/2.0, getVectorFromColor(rotCol).cast<float>());
        for (auto faceCol : RubikBase::RubikColors)
        {
            Vector3f faceAsVec = getVectorFromColor(faceCol).cast<float>();
            Vector3f resFaceAsVec = ax*faceAsVec;
            RubikColor dstCol = getColorFromVecf(resFaceAsVec);
            forwFunc[faceCol] = dstCol;
            //printf("rotate face %s color %s goes to color %s\n", colorName(rotCol).c_str(), colorName(faceCol).c_str(), colorName(dstCol).c_str());
        }
        m_PermutationBySideRotation[rotCol].setForward(forwFunc);
    }
}
