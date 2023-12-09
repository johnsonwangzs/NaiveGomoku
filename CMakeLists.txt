cmake_minimum_required(VERSION 3.26)
project(NaiveGomoku)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(NaiveGomoku main.cpp
        chessboard.h
        judge.h
        player.h
        scheduler.h
        utils.h
        scheduler.cpp
        utils.cpp
        player.cpp
        chessboard.cpp
        judge.cpp
)
