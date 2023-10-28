cmake_minimum_required(VERSION 3.26)
project(NaiveGomoku)

set(CMAKE_CXX_STANDARD 17)

add_executable(NaiveGomoku main.cpp
        chessboard.h
        judge.h
        player.h
        scheduler.h
        utils.h)
