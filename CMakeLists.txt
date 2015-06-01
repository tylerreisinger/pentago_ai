cmake_minimum_required(VERSION 2.4)
project(Pentago)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -static-libstdc++ -static-libgcc")

set(SOURCES
    ./src/Board.cpp
    ./src/PlayerController.cpp
    ./src/Pentago.cpp
    ./src/Move.cpp
    ./src/Enums.cpp
    ./src/HumanPlayerController.cpp
    ./src/RandomComputerController.cpp
    ./src/MinimaxComputerController.cpp
    ./src/MctsComputerController.cpp)

add_executable(pentago ./src/main.cpp ${SOURCES})
add_executable(arena ./src/Arena.cpp ${SOURCES})
