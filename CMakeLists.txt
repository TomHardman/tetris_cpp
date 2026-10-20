cmake_minimum_required(VERSION 3.16)
project(Tetris)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_PREFIX_PATH /opt/homebrew)

add_executable(Experimentation
    experimentation.cpp
)

add_executable(ExperimentationStdIn
    experimentationStdIn.cpp
)

add_executable(tetris
    tetris.cpp
    game.cpp
    components.cpp
)