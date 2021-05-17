cmake_minimum_required(VERSION 3.17)
project(multithreading_lab_3)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCL REQUIRED)
message(STATUS "OpenCL found: ${OpenCL_LIBRARY}")
function(build_with_openCL name)
    message("Building ${name} with OpenCL. Sources: ${ARGN}")
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} OpenCL::OpenCL)
endfunction()

file(GLOB SOURCE_FILES src/*.cpp src/*.*h*)

build_with_openCL(lab3 runners/main.cpp ${SOURCE_FILES})
build_with_openCL(lab3_unit_tests runners/unitTests.cpp ${SOURCE_FILES})