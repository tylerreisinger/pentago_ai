cmake_minimum_required(VERSION 2.4)
project(Pentago)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

set(SOURCES
    ./src/Board.cpp
    ./src/PlayerController.cpp
    ./src/Pentago.cpp
    ./src/Move.cpp
    ./src/Enums.cpp
    ./src/HumanPlayerController.cpp
    ./src/RandomComputerController.cpp
    ./src/MinimaxComputerController.cpp
    ./src/MctsComputerController.cpp
    ./src/ControllerFactory.cpp)

add_executable(pentago ./src/main.cpp ${SOURCES})
