#include "gtest/gtest.h"

#include "RubikBase.h"

#include <fstream>
#include <sstream>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <cereal/archives/binary.hpp> 
#include <cereal/archives/xml.hpp>

template <class T>
std::string objToStreamToString(T c)
{
    std::stringstream str;
    str << c;
    return str.str();
}

TEST(RubikBaseTest, TestColorVectors) {
    EXPECT_ANY_THROW(AsertFunc("here", "ther", 88));

    EXPECT_EQ(green, getColorFromVeci(Vector3i({1, 0, 0})));
    EXPECT_EQ(blue, getColorFromVeci(Vector3i({-1, 0, 0})));
    EXPECT_EQ(white, getColorFromVeci(Vector3i({0, 1, 0})));
    EXPECT_EQ(yellow, getColorFromVeci(Vector3i({0, -1, 0})));
    EXPECT_EQ(red, getColorFromVeci(Vector3i({0, 0, 1})));
    EXPECT_EQ(orange, getColorFromVeci(Vector3i({0, 0, -1})));

    EXPECT_EQ(getVectorFromCoord(EdgeCoord({white, blue})), Vector3i({-1, 1, 0}));

    EXPECT_EQ(noColor, getColorFromVeci(Vector3i({2, 0, 0})));
    EXPECT_EQ(noColor, getColorFromVeci(Vector3i({1, 1, 0})));
    EXPECT_EQ(noColor, getColorFromVeci(Vector3i({1, 2, 3})));

    EXPECT_EQ(objToStreamToString(green), "col(green)");
    EXPECT_EQ(objToStreamToString(blue), "col(blue)");
    EXPECT_EQ(objToStreamToString(white), "col(white)");
    EXPECT_EQ(objToStreamToString(yellow), "col(yellow)");
    EXPECT_EQ(objToStreamToString(red), "col(red)");
    EXPECT_EQ(objToStreamToString(orange), "col(orange)");
    EXPECT_EQ(objToStreamToString(noColor), "col(no Color)");

    EXPECT_EQ(colorName(green), "green");
    EXPECT_EQ(colorName(blue), "blue");
    EXPECT_EQ(colorName(white), "white");
    EXPECT_EQ(colorName(yellow), "yellow");
    EXPECT_EQ(colorName(red), "red");
    EXPECT_EQ(colorName(orange), "orange");
    EXPECT_EQ(colorName(noColor), "no Color");
    EXPECT_ANY_THROW(colorName((RubikColor)99));

    EXPECT_EQ(oppositeColor(green), blue);
    EXPECT_EQ(oppositeColor(blue), green);
    EXPECT_EQ(oppositeColor(white), yellow);
    EXPECT_EQ(oppositeColor(yellow), white);
    EXPECT_EQ(oppositeColor(red), orange);
    EXPECT_EQ(oppositeColor(orange), red);

    const auto & allColors = RubikBase::RubikColors();
    EXPECT_EQ(allColors.size(), 6);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), green), 1);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), blue), 1);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), white), 1);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), yellow), 1);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), red), 1);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), orange), 1);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), noColor), 0);
    EXPECT_EQ(std::count(allColors.begin(), allColors.end(), (RubikColor)33), 0);

    EXPECT_EQ(orientationName(WellOriented), "WellOriented");
    EXPECT_EQ(orientationName(Twist1), "Twist1");
    EXPECT_EQ(orientationName(Twist2), "Twist2");
    EXPECT_ANY_THROW(orientationName((RubikOrientation)98));

    EXPECT_EQ(objToStreamToString(WellOriented), "WellOriented");
    EXPECT_EQ(objToStreamToString(Twist1), "Twist1");
    EXPECT_EQ(objToStreamToString(Twist2), "Twist2");

    RubikColor aColor = red; 
    std::string serial_str;
    {
        boost::iostreams::back_insert_device<std::string> inserter(serial_str);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > outStream(inserter);
        cereal::XMLOutputArchive class_to_ss(outStream); 
        class_to_ss(aColor); 
    }

    RubikColor anotherColor; 
    {
        boost::iostreams::basic_array_source<char> inputDevice(serial_str.data(), serial_str.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > inStream(inputDevice);
        cereal::XMLInputArchive ss_to_vec(inStream); 
        ss_to_vec(anotherColor); 
    }
    EXPECT_EQ(aColor, anotherColor);

}