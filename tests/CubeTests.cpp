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
