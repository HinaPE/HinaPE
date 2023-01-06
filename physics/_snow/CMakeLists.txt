cmake_minimum_required(VERSION 3.18)
project(HINAPE_SNOW VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

find_package(CUDAToolkit)
if (CUDA_FOUND)

elseif ()
    return()
endif ()

add_executable(snow main.cpp)
target_include_directories(
        snow
        PUBLIC
        ../../kasumi/backends/deps/igl
)