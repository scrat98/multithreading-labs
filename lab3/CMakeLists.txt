cmake_minimum_required(VERSION 3.17)
project(multithreading_lab_3)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCL REQUIRED)
message(STATUS "OpenCL found. Lib: ${OpenCL_LIBRARY}, Headers: ${OpenCL_INCLUDE_DIR}")
function(build_with_openCL name)
    message("Building ${name} with OpenCL. Sources: ${ARGN}")
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} OpenCL::OpenCL)
endfunction()

SET(KERNEL_SOURCE_FOLDER src/kernels)
SET(KERNEL_ABSOLUTE_SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_SOURCE_FOLDER})
file(GLOB KERNELS_FILES RELATIVE ${KERNEL_ABSOLUTE_SOURCE_FOLDER} ${KERNEL_SOURCE_FOLDER}/*.cl)
foreach (kernel_file ${KERNELS_FILES})
    message("Copying kernel ${kernel_file}")
    configure_file(${KERNEL_SOURCE_FOLDER}/${kernel_file} ${CMAKE_CURRENT_BINARY_DIR}/kernels/${kernel_file} COPYONLY)
endforeach ()

file(GLOB SOURCE_FILES src/*.cpp src/*.*h*)
build_with_openCL(lab3 runners/main.cpp ${SOURCE_FILES})
build_with_openCL(lab3_unit_tests runners/unitTests.cpp ${SOURCE_FILES})