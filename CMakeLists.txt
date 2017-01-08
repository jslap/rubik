cmake_minimum_required(VERSION 3.5.1)

include_directories ("/usr/local/include" "/opt/local/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(RubikLib
    RubikBase.cpp
    ColorPermutation.cpp
    ColorPermMap.cpp
    Cubelet.cpp
    Cube.cpp
    CubeHandler.cpp
CubeSolver.cpp
DummyCubeSolver.cpp)
