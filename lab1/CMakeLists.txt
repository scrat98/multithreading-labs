cmake_minimum_required(VERSION 3.17)
project(multithreading_lab_1)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

set(SOURCE_FILES main.cpp)
add_executable(lab1 ${SOURCE_FILES})