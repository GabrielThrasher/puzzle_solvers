cmake_minimum_required(VERSION 3.22)
project(puzzle_solvers)

set(CMAKE_CXX_STANDARD 14)
set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(patrix/src)

add_executable(puzzle_solvers
        ../patrix/src/main.cpp
        ../patrix/src/puzzle_piece.h
        ../patrix/src/puzzle_piece.cpp
        ../patrix/src/puzzle.h
        ../patrix/src/puzzle.cpp
        ../patrix/src/utilities.cpp
)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(puzzle_solvers ${OpenCV_LIBS})
