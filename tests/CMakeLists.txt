cmake_minimum_required(VERSION 3.5.1)

include_directories ("../third/googletest/googletest/include")
include_directories ("../third/cereal/include")
include_directories ("../RubikLib")

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

add_subdirectory (../RubikLib  ./_Lib)

add_subdirectory (../third/googletest ./_gtest)

# add the executable
add_executable (Rubik-Test 
    Rubik-Test.cpp 
    RubikBaseTests.cpp
    CubeTests.cpp 
    DummySolveTests.cpp 
    PermutationTests.cpp 
    ColorPermMapTests.cpp)
target_link_libraries (Rubik-Test RubikLib)
target_link_libraries (Rubik-Test gtest)
