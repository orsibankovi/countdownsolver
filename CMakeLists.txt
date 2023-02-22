cmake_minimum_required(VERSION 3.23)
project(nhf3skeleton)

set(CMAKE_CXX_STANDARD 20)

add_executable(nhf3skeleton main.cpp countdownsolver.hpp solutioninterface.hpp countdownsolver.hpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic")
