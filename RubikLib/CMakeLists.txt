cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")

SET(COVERAGE OFF CACHE BOOL "Compile tests with coverage info")
if (COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage -fprofile-arcs -ftest-coverage")
endif()    

SET(TIDY OFF CACHE BOOL "Compile tests with clang tidy")

if(TIDY)
    find_program(
            CLANG_TIDY_EXE
            NAMES "clang-tidy" "clang-tidy-6.0"
            DOC "Path to clang-tidy executable"
        )
endif()

if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")

    set(LOCAL_DISABLED_TESTS "")

    #   ########## disable fuchsia-* not relevant for us.
    set(LOCAL_DISABLED_TESTS "${LOCAL_DISABLED_TESTS},-fuchsia-*")

    # flags unused include path or lib path, or libs that are not used.
    set(LOCAL_DISABLED_TESTS "${LOCAL_DISABLED_TESTS},-clang-diagnostic-unused-command-line-argument*")

    # Checks the usage of non-constant references in function parameters.
    # OK.
    set(LOCAL_DISABLED_TESTS "${LOCAL_DISABLED_TESTS},-google-runtime-references")

    set(LOCAL_WARNING_AS_ERROR "-warnings-as-errors=*")

    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*${LOCAL_DISABLED_TESTS}")
    set(DO_CLANG_TIDY_WAE "${DO_CLANG_TIDY}" "${LOCAL_WARNING_AS_ERROR}")
    # set(DO_CLANG_TIDY_WAE "${DO_CLANG_TIDY}")
endif()


include_directories (SYSTEM "third/cereal/include")
include_directories (SYSTEM "third/range/include")

find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})
MESSAGE( STATUS "EIGEN3_INCLUDE_DIR:         " ${EIGEN3_INCLUDE_DIR} )

find_package(boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
MESSAGE( STATUS "Boost_INCLUDE_DIR:         " ${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror")

include_directories (".")


add_library(RubikLib
    RubikAssert.cpp
    RubikBase.cpp
    ColorPermutation.cpp
    ColorPermMap.cpp
    Cubelet.cpp
    Cube.cpp
    CubeHandler.cpp
    Solver/CubeSolver.cpp
    Solver/ExportCubeSolver.cpp
    Solver/ExportSolverConvert.cpp
    Solver/DummyCubeSolver.cpp
    RubikBase.h
    ColorPermutation.h
    ColorPermMap.h
    Cubelet.h
    Cube.h
    CubeHandler.h
    Solver/CubeSolver.h
    Solver/DummyCubeSolver.h
)

# Solver/FridrichSolver.h
# Solver/GenBruteSolve.h
# Solver/ThistleSolver.h
# Solver/FridrichSolver.cpp
# Solver/GenBruteSolve.cpp
# Solver/ThistleSolver.cpp


if(CLANG_TIDY_EXE)
  set_target_properties(
    RubikLib PROPERTIES
    CXX_CLANG_TIDY "${DO_CLANG_TIDY_WAE}"
  )
endif()