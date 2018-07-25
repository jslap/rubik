cmake_minimum_required(VERSION 3.5.1)

include_directories ("../third/googletest/googletest/include")
include_directories ("../third/cereal/include")
include_directories ("../third/range/include")
include_directories ("../RubikLib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")

SET(COVERAGE OFF CACHE BOOL "Compile tests with coverage info")

if (COVERAGE)
    if ( (CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g --coverage -fprofile-arcs -ftest-coverage")
    endif()
endif()

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

add_subdirectory (../RubikLib  ./_Lib)

add_subdirectory (../third/googletest ./_gtest)

# add the executable
add_executable (Rubik-Test 
    Rubik-Test.cpp 
    RubikBaseTests.cpp
    CubeTests.cpp 
    DummySolveTests.cpp 
    ColorPermutationTests.cpp 
    ColorPermMapTests.cpp)
target_link_libraries (Rubik-Test RubikLib)
target_link_libraries (Rubik-Test gtest)
