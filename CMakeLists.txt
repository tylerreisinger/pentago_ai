cmake_minimum_required(VERSION 2.6)
project(Pentago)

set(SOURCES
    ./src/Board.cpp
    ./src/main.cpp
    ./src/PlayerController.cpp
    ./src/Pentago.cpp
    ./src/Move.cpp
    ./src/Enums.cpp
    ./src/HumanPlayerController.cpp)

add_executable(pentago ${SOURCES})
