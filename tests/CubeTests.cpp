#include "gtest/gtest.h"
#include "Cube.h"

TEST(EdgeCubeTest, BasicEdgeCubeTest)
{
    std::array<RubikColor, 2> whiteRedArray({white, red});
    std::array<RubikColor, 2> whiteRedFlippedArray({red, white});
    EdgeCube whiteRed(whiteRedArray);
    EdgeCube whiteRed2(white, red);
    EXPECT_EQ(whiteRed, whiteRed2) << "cubelet contructor should be equivalent";

    // up to 4 rotation
    EdgeCube whiteRedRotateByRed = whiteRed;
    whiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_NE(whiteRed, whiteRedRotateByRed) << "cubelet should be rotated";
    whiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_NE(whiteRed, whiteRedRotateByRed) << "cubelet should be rotated";
    whiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_NE(whiteRed, whiteRedRotateByRed) << "cubelet should be rotated";
    whiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_EQ(whiteRed, whiteRedRotateByRed) << "cubelet should be back to origin";

    // Check properties for solved cubelet.
    EXPECT_TRUE(whiteRed.hasInColor(red)) << "Should have its color in its color";
    EXPECT_TRUE(whiteRed.hasInColor(white)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInColor(blue)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInColor(orange)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInColor(green)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInColor(yellow)) << "Should have its color in its color";
    EXPECT_TRUE(whiteRed.hasInPosition(red)) << "Should have its color in its color";
    EXPECT_TRUE(whiteRed.hasInPosition(white)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInPosition(blue)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInPosition(orange)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInPosition(green)) << "Should have its color in its color";
    EXPECT_FALSE(whiteRed.hasInPosition(yellow)) << "Should have its color in its color";
    
    EXPECT_TRUE(whiteRed.isAtSolvedPosition()) << "Should be at solve pos.";
    EXPECT_EQ(whiteRed.getPosition(), whiteRedArray) << "position should be as set.";
    EXPECT_EQ(whiteRed.getColor(), whiteRedArray) << "color should be as set.";
    EXPECT_EQ(whiteRed.getOrientation(), WellOriented) << "position should be as set.";
    EXPECT_EQ(whiteRed.positionForColor(white), white) << "pos and color should be same.";
    EXPECT_EQ(whiteRed.positionForColor(red), red) << "pos and color should be same.";
    EXPECT_ANY_THROW(whiteRed.positionForColor(blue));
    EXPECT_EQ(whiteRed.colorForPosition(white), white) << "pos and color should be same.";
    EXPECT_EQ(whiteRed.colorForPosition(red), red) << "pos and color should be same.";
    EXPECT_ANY_THROW(whiteRed.colorForPosition(blue));
    EXPECT_EQ(whiteRed.getColorNot(white), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRed.getColorNot(red), white) << "pos and color should be same.";
    EXPECT_TRUE(whiteRed.getColorNot(blue) == white || whiteRed.getColorNot(blue) == red) << "pos and color should be same.";
    EXPECT_EQ(whiteRed.getPositionNot(white), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRed.getPositionNot(red), white) << "pos and color should be same.";
    EXPECT_TRUE(whiteRed.getPositionNot(blue) == white || whiteRed.getPositionNot(blue) == red) << "pos and color should be same.";


    //rotate red and check properties. wrong place and all
    whiteRedRotateByRed = whiteRed;
    whiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_FALSE(whiteRedRotateByRed.isAtSolvedPosition()) << "Should be at solve pos.";
    EXPECT_FALSE(whiteRedRotateByRed.isSolved()) << "Should be at solve pos.";
    EXPECT_NE(whiteRedRotateByRed.getPosition(), whiteRedArray) << "position should be as set.";
    EXPECT_EQ(whiteRedRotateByRed.getColor(), whiteRedArray) << "color should be as set.";
    EXPECT_EQ(whiteRedRotateByRed.positionForColor(white), green) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.positionForColor(red), red) << "pos and color should be same.";
    EXPECT_ANY_THROW(whiteRedRotateByRed.positionForColor(blue));
    EXPECT_EQ(whiteRedRotateByRed.colorForPosition(green), white) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.colorForPosition(red), red) << "pos and color should be same.";
    EXPECT_ANY_THROW(whiteRedRotateByRed.colorForPosition(white));
    EXPECT_EQ(whiteRedRotateByRed.getColorNot(white), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.getColorNot(red), white) << "pos and color should be same.";
    EXPECT_TRUE(whiteRedRotateByRed.getColorNot(blue) == white || whiteRedRotateByRed.getColorNot(blue) == red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.getPositionNot(green), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.getPositionNot(red), green) << "pos and color should be same.";
    EXPECT_TRUE(whiteRedRotateByRed.getPositionNot(white) == green || whiteRedRotateByRed.getPositionNot(white) == red) << "pos and color should be same.";


    // rotate red - green - white check properties right place bu not oriented.
    whiteRedRotateByRed = whiteRed;
    whiteRedRotateByRed.rotate(red, true, 1);
    whiteRedRotateByRed.rotate(green, true, 1);
    whiteRedRotateByRed.rotate(white, true, 1);
    EXPECT_TRUE(whiteRedRotateByRed.isAtSolvedPosition()) << "Should be at solve pos.";
    EXPECT_FALSE(whiteRedRotateByRed.isSolved()) << "Should be at solve pos.";
    EXPECT_EQ(whiteRedRotateByRed.getPosition(), whiteRedFlippedArray) << "position should be as set.";
    EXPECT_EQ(whiteRedRotateByRed.getColor(), whiteRedArray) << "color should be as set.";
    EXPECT_NE(whiteRedRotateByRed.getOrientation(), WellOriented) << "position should be as set.";
    EXPECT_EQ(whiteRedRotateByRed.positionForColor(white), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.positionForColor(red), white) << "pos and color should be same.";
    EXPECT_ANY_THROW(whiteRedRotateByRed.positionForColor(blue));
    EXPECT_EQ(whiteRedRotateByRed.colorForPosition(white), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.colorForPosition(red), white) << "pos and color should be same.";
    EXPECT_ANY_THROW(whiteRedRotateByRed.colorForPosition(blue));
    EXPECT_EQ(whiteRedRotateByRed.getColorNot(white), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.getColorNot(red), white) << "pos and color should be same.";
    EXPECT_TRUE(whiteRedRotateByRed.getColorNot(blue) == white || whiteRedRotateByRed.getColorNot(blue) == red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.getPositionNot(white), red) << "pos and color should be same.";
    EXPECT_EQ(whiteRedRotateByRed.getPositionNot(red), white) << "pos and color should be same.";
    EXPECT_TRUE(whiteRedRotateByRed.getPositionNot(blue) == white || whiteRedRotateByRed.getPositionNot(blue) == red) << "pos and color should be same.";
}

TEST(CornerCubeTest, BasicCornerCubeTest)
{
    std::array<RubikColor, 3> blueWhiteRedArray({blue, white, red});
    std::array<RubikColor, 3> blueWhiteRedFlippedArray({blue, red, white});
    CornerCube blueWhiteRed(blueWhiteRedArray);
    CornerCube blueWhiteRed2(blue, white, red);
    EXPECT_EQ(blueWhiteRed, blueWhiteRed2) << "cubelet contructor should be equivalent";

    // up to 4 rotation
    CornerCube blueWhiteRedRotateByRed = blueWhiteRed;
    blueWhiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_NE(blueWhiteRed, blueWhiteRedRotateByRed) << "cubelet should be rotated";
    blueWhiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_NE(blueWhiteRed, blueWhiteRedRotateByRed) << "cubelet should be rotated";
    blueWhiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_NE(blueWhiteRed, blueWhiteRedRotateByRed) << "cubelet should be rotated";
    blueWhiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_EQ(blueWhiteRed, blueWhiteRedRotateByRed) << "cubelet should be back to origin";

    // Check properties for solved cubelet.
    EXPECT_TRUE(blueWhiteRed.hasInColor(red)) << "Should have its color in its color";
    EXPECT_TRUE(blueWhiteRed.hasInColor(white)) << "Should have its color in its color";
    EXPECT_TRUE(blueWhiteRed.hasInColor(blue)) << "Should have its color in its color";
    EXPECT_FALSE(blueWhiteRed.hasInColor(orange)) << "Should have its color in its color";
    EXPECT_FALSE(blueWhiteRed.hasInColor(green)) << "Should have its color in its color";
    EXPECT_FALSE(blueWhiteRed.hasInColor(yellow)) << "Should have its color in its color";
    EXPECT_TRUE(blueWhiteRed.hasInPosition(red)) << "Should have its color in its color";
    EXPECT_TRUE(blueWhiteRed.hasInPosition(white)) << "Should have its color in its color";
    EXPECT_TRUE(blueWhiteRed.hasInPosition(blue)) << "Should have its color in its color";
    EXPECT_FALSE(blueWhiteRed.hasInPosition(orange)) << "Should have its color in its color";
    EXPECT_FALSE(blueWhiteRed.hasInPosition(green)) << "Should have its color in its color";
    EXPECT_FALSE(blueWhiteRed.hasInPosition(yellow)) << "Should have its color in its color";
    
    EXPECT_TRUE(blueWhiteRed.isAtSolvedPosition()) << "Should be at solve pos.";
    EXPECT_TRUE(blueWhiteRed.isSolved()) << "Should be at solve pos.";
    EXPECT_TRUE(isSameCubeColor(blueWhiteRed.getPosition(), blueWhiteRedArray)) << "position should be as set.";
    EXPECT_EQ(blueWhiteRed.getColor(), blueWhiteRedArray) << "color should be as set.";
    EXPECT_EQ(blueWhiteRed.getOrientation(), WellOriented) << "position should be as set.";
    EXPECT_EQ(blueWhiteRed.positionForColor(white), white) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRed.positionForColor(red), red) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRed.positionForColor(blue), blue) << "pos and color should be same.";
    EXPECT_ANY_THROW(blueWhiteRed.positionForColor(orange));
    EXPECT_EQ(blueWhiteRed.colorForPosition(white), white) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRed.colorForPosition(red), red) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRed.colorForPosition(blue), blue) << "pos and color should be same.";
    EXPECT_ANY_THROW(blueWhiteRed.colorForPosition(orange));
    {
        auto notWhiteCol = blueWhiteRed.getColorNot(white);
        auto notRedCol = blueWhiteRed.getColorNot(red);
        auto notBlueCol = blueWhiteRed.getColorNot(blue);
        auto notOrangeCol = blueWhiteRed.getColorNot(orange);
        EXPECT_TRUE(notWhiteCol == red || notWhiteCol == blue) << "pos and color should be same.";
        EXPECT_TRUE(notRedCol == white || notRedCol == blue) << "pos and color should be same.";
        EXPECT_TRUE(notBlueCol == red || notBlueCol == white) << "pos and color should be same.";
        EXPECT_TRUE(notOrangeCol == red || notOrangeCol == blue || notOrangeCol == white) << "pos and color should be same.";
        auto notWhitePos = blueWhiteRed.getPositionNot(white);
        auto notRedPos = blueWhiteRed.getPositionNot(red);
        auto notBluePos = blueWhiteRed.getPositionNot(blue);
        auto notOrangePos = blueWhiteRed.getPositionNot(orange);
        EXPECT_TRUE(notWhitePos == red || notWhitePos == blue) << "pos and color should be same.";
        EXPECT_TRUE(notRedPos == white || notRedPos == blue) << "pos and color should be same.";
        EXPECT_TRUE(notBluePos == red || notBluePos == white) << "pos and color should be same.";
        EXPECT_TRUE(notOrangePos == red || notOrangePos == blue || notOrangePos == white) << "pos and color should be same.";        
    }

    //rotate red and check properties. wrong place and all
    blueWhiteRedRotateByRed = blueWhiteRed;
    blueWhiteRedRotateByRed.rotate(red, true, 1);
    EXPECT_FALSE(blueWhiteRedRotateByRed.isAtSolvedPosition()) << "Should be at solve pos.";
    EXPECT_FALSE(blueWhiteRedRotateByRed.isSolved()) << "Should be at solve pos.";
    EXPECT_TRUE(isSameCubeColor(blueWhiteRedRotateByRed.getPosition(), (std::array<RubikColor, 3>({white, green, red})))) << "position should be as set.";
    EXPECT_EQ(blueWhiteRedRotateByRed.getColor(), blueWhiteRedArray) << "color should be as set.";
    EXPECT_EQ(blueWhiteRedRotateByRed.getOrientation(), Twist1) << "position should be as set.";
    EXPECT_EQ(blueWhiteRedRotateByRed.positionForColor(white), green) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRed.positionForColor(red), red) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRed.positionForColor(blue), white) << "pos and color should be same.";
    EXPECT_ANY_THROW(blueWhiteRedRotateByRed.positionForColor(orange));
    EXPECT_EQ(blueWhiteRedRotateByRed.colorForPosition(white), blue) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRed.colorForPosition(red), red) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRed.colorForPosition(green), white) << "pos and color should be same.";
    EXPECT_ANY_THROW(blueWhiteRedRotateByRed.colorForPosition(orange));
    {
        auto notWhiteCol = blueWhiteRedRotateByRed.getColorNot(white);
        auto notRedCol = blueWhiteRedRotateByRed.getColorNot(red);
        auto notBlueCol = blueWhiteRedRotateByRed.getColorNot(blue);
        auto notOrangeCol = blueWhiteRedRotateByRed.getColorNot(orange);
        EXPECT_TRUE(notWhiteCol == red || notWhiteCol == blue) << "pos and color should be same.";
        EXPECT_TRUE(notRedCol == white || notRedCol == blue) << "pos and color should be same.";
        EXPECT_TRUE(notBlueCol == red || notBlueCol == white) << "pos and color should be same.";
        EXPECT_TRUE(notOrangeCol == red || notOrangeCol == blue || notOrangeCol == white) << "pos and color should be same.";
        auto notWhitePos = blueWhiteRedRotateByRed.getPositionNot(white);
        auto notGreenPos = blueWhiteRedRotateByRed.getPositionNot(green);
        auto notRedPos = blueWhiteRedRotateByRed.getPositionNot(red);
        auto notOrangePos = blueWhiteRedRotateByRed.getPositionNot(orange);
        EXPECT_TRUE(notWhitePos == red || notWhitePos == green) << "pos and color should be same.";
        EXPECT_TRUE(notGreenPos == red || notGreenPos == white) << "pos and color should be same.";
        EXPECT_TRUE(notRedPos == white || notRedPos == green) << "pos and color should be same.";
        EXPECT_TRUE(notOrangePos == red || notOrangePos == green || notOrangePos == white) << "pos and color should be same.";
    }

    // rotate red - green - white check properties right place bu not oriented.
    CornerCube blueWhiteRedRotateByRedBlue = blueWhiteRed;
    blueWhiteRedRotateByRedBlue.rotate(red, false, 1);
    blueWhiteRedRotateByRedBlue.rotate(blue, false, 1);
    EXPECT_TRUE(blueWhiteRedRotateByRedBlue.isAtSolvedPosition()) << "Should be at solve pos.";
    EXPECT_FALSE(blueWhiteRedRotateByRedBlue.isSolved()) << "Should be at solve pos.";
    EXPECT_TRUE(isSameCubeColor(blueWhiteRedRotateByRedBlue.getPosition(), blueWhiteRedArray)) << "position should be as set.";
    EXPECT_EQ(blueWhiteRedRotateByRedBlue.getColor(), blueWhiteRedArray) << "color should be as set.";
    EXPECT_NE(blueWhiteRedRotateByRedBlue.getOrientation(), WellOriented) << "position should be as set.";
    EXPECT_EQ(blueWhiteRedRotateByRedBlue.positionForColor(white), blue) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRedBlue.positionForColor(red), white) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRedBlue.positionForColor(blue), red) << "pos and color should be same.";
    EXPECT_ANY_THROW(blueWhiteRedRotateByRedBlue.positionForColor(orange));
    EXPECT_EQ(blueWhiteRedRotateByRedBlue.colorForPosition(white), red) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRedBlue.colorForPosition(red), blue) << "pos and color should be same.";
    EXPECT_EQ(blueWhiteRedRotateByRedBlue.colorForPosition(blue), white) << "pos and color should be same.";
    EXPECT_ANY_THROW(blueWhiteRedRotateByRedBlue.colorForPosition(orange));
    {
        auto notWhiteCol = blueWhiteRedRotateByRedBlue.getColorNot(white);
        auto notRedCol = blueWhiteRedRotateByRedBlue.getColorNot(red);
        auto notBlueCol = blueWhiteRedRotateByRedBlue.getColorNot(blue);
        auto notOrangeCol = blueWhiteRedRotateByRedBlue.getColorNot(orange);
        EXPECT_TRUE(notWhiteCol == red || notWhiteCol == blue) << "pos and color should be same.";
        EXPECT_TRUE(notRedCol == white || notRedCol == blue) << "pos and color should be same.";
        EXPECT_TRUE(notBlueCol == red || notBlueCol == white) << "pos and color should be same.";
        EXPECT_TRUE(notOrangeCol == red || notOrangeCol == blue || notOrangeCol == white) << "pos and color should be same.";
        auto notWhitePos = blueWhiteRedRotateByRedBlue.getPositionNot(white);
        auto notBluePos = blueWhiteRedRotateByRedBlue.getPositionNot(blue);
        auto notRedPos = blueWhiteRedRotateByRedBlue.getPositionNot(red);
        auto notOrangePos = blueWhiteRedRotateByRedBlue.getPositionNot(orange);
        EXPECT_TRUE(notWhitePos == red || notWhitePos == blue) << "pos and color should be same.";
        EXPECT_TRUE(notBluePos == red || notBluePos == white) << "pos and color should be same.";
        EXPECT_TRUE(notRedPos == white || notRedPos == blue) << "pos and color should be same.";
        EXPECT_TRUE(notOrangePos == red || notOrangePos == blue || notOrangePos == white) << "pos and color should be same.";
        auto notOrangeOrRedPos = blueWhiteRedRotateByRedBlue.getPositionNot(orange, red);
        auto notBlueOrRedPos = blueWhiteRedRotateByRedBlue.getPositionNot(blue, red);
        EXPECT_TRUE(notOrangeOrRedPos == red || notOrangeOrRedPos == blue || notOrangeOrRedPos == white) << "pos and color should be same.";
        EXPECT_EQ(notBlueOrRedPos, white)  << "pos and color should be same.";
    }
    EXPECT_ANY_THROW(blueWhiteRedRotateByRedBlue.getColorIn(orange, yellow));
    EXPECT_ANY_THROW(blueWhiteRedRotateByRedBlue.getPositionIn(orange, yellow));
}

TEST(CubeTest, moveCubeTest)
{

    Cube c1;
    Cube c2;
    EXPECT_EQ(c1, c2) << "default cube should be equal";
    c2.rotate(white, true);
    EXPECT_NE(c1, c2) << "after one rot, should not be equal";
    c2.rotate(white, false);
    EXPECT_EQ(c1, c2) << "cube should be equal after inverse rot";
    Cube c3;
    c3.rotate(white, false);
    c3.rotate(white, false);
    c3.rotate(white, false);
    c3.rotate(white, false);
    EXPECT_EQ(c1, c3) << "cube should be equal after 4 identical rot";

}
auto allCornerWellOriented = [](const Cube& c){
    auto cornerWellOriented = [](const CornerCube& c){return c.getOrientation() == WellOriented;};
    return std::all_of(c.getCorners().begin(), c.getCorners().end(), cornerWellOriented);
};

TEST(CubeTest, OrientationTest)
{
    Cube c;
    c.rotate(red, false, 1);
    EXPECT_FALSE(allCornerWellOriented(c));

    c.rotate(red, true, 1);
    EXPECT_TRUE(allCornerWellOriented(c));

}

TEST(CubeTest, OrientationTest2)
{
    Cube c;
    c.rotate(red, false, 1);
    c.rotate(red, false, 1);
    EXPECT_TRUE(allCornerWellOriented(c));
}


TEST(CubeTest, solvedTest)
{
    Cube c;
    EXPECT_TRUE(c.isSolved());
    c.rotate(red, true);
    EXPECT_FALSE(c.isSolved());
}

TEST(CubeTest, OneTestTest)
{
    Cube badCubeSwapOneCorner;
    badCubeSwapOneCorner.swapSinglePiece(CornerCoord({{orange, blue, white}}));
    EXPECT_FALSE(badCubeSwapOneCorner.isValidCube());
}
TEST(CubeTest, solvableTest)
{
    Cube c;
    EXPECT_TRUE(c.isValidCube());
    for (int i = 0; i<5; i++)
    {
        c.rotate(white, false);
        c.rotate(red, true);
        c.rotate(blue, true);
        c.rotate(yellow, false);
    }
    EXPECT_TRUE(c.isValidCube());

    Cube badCubeRotateCorner;
    badCubeRotateCorner.rotateSinglePiece(CornerCoord({{white, orange, blue}}), true);
    EXPECT_FALSE(badCubeRotateCorner.isValidCube());
    badCubeRotateCorner.rotateSinglePiece(CornerCoord({{white, orange, blue}}), false);
    badCubeRotateCorner.rotateSinglePiece(CornerCoord({{white, orange, blue}}), false);
    EXPECT_FALSE(badCubeRotateCorner.isValidCube());

    Cube badMixedCubeRotateCorner = c;
    badMixedCubeRotateCorner.rotateSinglePiece(CornerCoord({{white, orange, blue}}), true);
    EXPECT_FALSE(badMixedCubeRotateCorner.isValidCube());

    Cube badCubeSwapOneCorner;
    badCubeSwapOneCorner.swapSinglePiece(CornerCoord({{orange, blue, white}}));
    EXPECT_FALSE(badCubeSwapOneCorner.isValidCube());

    Cube badMixedCubeSwapOneCorner = c;
    badMixedCubeSwapOneCorner.swapSinglePiece(CornerCoord({{orange, blue, white}}));
    EXPECT_FALSE(badMixedCubeSwapOneCorner.isValidCube());

    Cube badCubeFlipEdge;
    badCubeFlipEdge.flipEdge(EdgeCoord({{white, blue}}));
    EXPECT_FALSE(badCubeFlipEdge.isValidCube());

    Cube badMixedCubeFlipEdge = c;
    badMixedCubeFlipEdge.flipEdge(EdgeCoord({{white, blue}}));
    EXPECT_FALSE(badMixedCubeFlipEdge.isValidCube());

    Cube badCubeSwapEdges;
    badCubeSwapEdges.swapEdges(EdgeCoord({{white, blue}}), EdgeCoord({{yellow, green}}));
    EXPECT_FALSE(badCubeSwapEdges.isValidCube());

    Cube badMixedCubeSwapEdges = c;
    badMixedCubeSwapEdges.swapEdges(EdgeCoord({{white, blue}}), EdgeCoord({{yellow, green}}));
    EXPECT_FALSE(badMixedCubeSwapEdges.isValidCube());

    Cube badCubeSwapCorners;
    badCubeSwapCorners.swapCorners(CornerCoord({{white, orange, blue}}), CornerCoord({{yellow, red, green}}));
    EXPECT_FALSE(badCubeSwapCorners.isValidCube());

    Cube badMixedCubeSwapCorners = c;
    badMixedCubeSwapCorners.swapCorners(CornerCoord({{white, orange, blue}}), CornerCoord({{yellow, red, green}}));
    EXPECT_FALSE(badMixedCubeSwapCorners.isValidCube());
}

#include <fstream>
#include <sstream>
#include <cereal/archives/xml.hpp>

TEST(SerializationCubeTest, SerializeTest)
{
    Cube defCube;
    Cube c;
    for (int i = 0; i<5; i++)
    {
        c.rotate(white, false);
        c.rotate(red, true);
        c.rotate(blue, true);
        c.rotate(yellow, false);
    }

    {
        std::ofstream os("defCube.xml");
        cereal::XMLOutputArchive archive(os);
        archive( defCube);
    }

    {
        std::ofstream os("mixedCube.xml");
        cereal::XMLOutputArchive archive(os);
        archive( c);
    }

    Cube readDefCube;
    {
        std::ifstream istr("defCube.xml");
        cereal::XMLInputArchive iarchive(istr); // Create an input archive
        iarchive(readDefCube); // Read the data from the archive
    }
    EXPECT_EQ(defCube, readDefCube);

    Cube readMixedCube;
    {
        std::ifstream istr("mixedCube.xml");
        cereal::XMLInputArchive iarchive(istr); // Create an input archive
        iarchive(readMixedCube); // Read the data from the archive
    }
    EXPECT_EQ(c, readMixedCube);
}
