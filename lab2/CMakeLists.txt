cmake_minimum_required(VERSION 3.17)
project(multithreading_lab_2)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

file(GLOB SOURCE_FILES src/*.cpp src/*.*h*)

add_executable(lab2 runners/main.cpp ${SOURCE_FILES})
add_executable(lab2_unit_tests runners/unitTests.cpp ${SOURCE_FILES})