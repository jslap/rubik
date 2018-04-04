cmake_minimum_required(VERSION 3.5.1)

include_directories ("/usr/local/include" "/opt/local/include" "third/cereal/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

add_library(RubikLib
    RubikBase.cpp
    ColorPermutation.cpp
    ColorPermMap.cpp
    Cubelet.cpp
    Cube.cpp
    CubeHandler.cpp
    CubeSolver.cpp
    DummyCubeSolver.cpp
    FridrichSolver.cpp
    GenBruteSolve.cpp
    ThistleSolver.cpp
    RubikBase.h
    ColorPermutation.h
    ColorPermMap.h
    Cubelet.h
    Cube.h
    CubeHandler.h
    CubeSolver.h
    DummyCubeSolver.h
    FridrichSolver.h
    GenBruteSolve.h
    ThistleSolver.h
)
