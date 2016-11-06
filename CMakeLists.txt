cmake_minimum_required(VERSION 3.6)
project(FPS_Matrixmultiplikation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main.c src/Matrix.c src/include/Matrix.h src/matrixmultiplication.c src/include/matrixmultiplication.h)

add_executable(FPS_Matrixmultiplikation ${SOURCE_FILES})