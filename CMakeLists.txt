cmake_minimum_required(VERSION 3.6)
project(FPS_Matrixmultiplikation)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")


#set(CMAKE_CXX_FLAGS -std=c99 -Wall -lm -mavx -O2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -lm -mavx")



set(SOURCE_FILES src/main.c src/Matrix.c src/include/Matrix.h src/OctaMatrix.c src/include/OctaMatrix.h)

include_directories(src/include)

add_executable(FPS_Matrixmultiplikation ${SOURCE_FILES})