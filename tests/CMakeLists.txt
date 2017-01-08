cmake_minimum_required(VERSION 3.5.1)

include_directories ("../third/googletest/googletest/include" "..")
include_directories ("/usr/local/include" "/opt/local/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory (../  ./_Lib)

add_subdirectory (../third/googletest ./_gtest)

# add the executable
add_executable (Rubik-Test Rubik-Test.cpp CubeTests.cpp DummySolveTests.cpp)
target_link_libraries (Rubik-Test RubikLib)
target_link_libraries (Rubik-Test gtest)
