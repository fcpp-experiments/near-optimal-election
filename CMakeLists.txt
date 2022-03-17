# minimum required version of CMake
cmake_minimum_required(VERSION 3.9)
# avoid building internal tests of the FCPP library
option(FCPP_INTERNAL_TESTS "Build internal tests for FCPP." OFF)
# load the FCPP sub-module
add_subdirectory(./fcpp/src)
# CMake macro setting up the FCPP library
fcpp_setup()

# project description
project(
    election
    VERSION 0.0.0
    DESCRIPTION "Simulation of a Near-Optimal Leader Election Algorithm."
)

# target declaration
fcpp_target(./run/parameter.cpp OFF)
fcpp_target(./run/batch.cpp     OFF)
fcpp_target(./run/graphic.cpp   ON)
