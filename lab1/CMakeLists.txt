cmake_minimum_required(VERSION 3.17)
project(multithreading_lab_1)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(SOURCE_FILES main.cpp)

add_executable(lab1 ${SOURCE_FILES})