#include "gtest/gtest.h"
#include "Cube.h"


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
#include <cereal/archives/json.hpp>

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
        std::ofstream os("defCube.json");
        cereal::JSONOutputArchive archive(os);
        archive( defCube);
    }

    {
        std::ofstream os("mixedCube.json");
        cereal::JSONOutputArchive archive(os);
        archive( c);
    }

    Cube readDefCube;
    {
        std::ifstream istr("defCube.json");
        cereal::JSONInputArchive iarchive(istr); // Create an input archive
        iarchive(readDefCube); // Read the data from the archive
    }
    EXPECT_EQ(defCube, readDefCube);

    Cube readMixedCube;
    {
        std::ifstream istr("mixedCube.json");
        cereal::JSONInputArchive iarchive(istr); // Create an input archive
        iarchive(readMixedCube); // Read the data from the archive
    }
    EXPECT_EQ(c, readMixedCube);
}
