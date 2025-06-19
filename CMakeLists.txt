# CMakeList.txt : CMake project for GeorgeWuPathFinding02, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("GeorgeWuPathFinding02")

# Add source to this project's executable.
add_executable (GeorgeWuPathFinding02 "GeorgeWuPathFinding.cpp" "GeorgeWuPathFinding.h"  "PathFinding.h" "PathFinding.cpp")

# TODO: Add tests and install targets if needed.
