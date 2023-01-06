cmake_minimum_required(VERSION 3.18)
project(HINAPE_SNOW VERSION 1.40 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

find_package(CUDAToolkit)
if (CUDA_FOUND)
    enable_language(CUDA)
    cuda_select_nvcc_arch_flags(CUDA_NVCC_FLAGS "7.5" "Turing")
    # We will use CUDA_TARGET_OPTIONS instead of CUDA_NVCC_FLAGS to be explicit.
    # start with architecture options
    list(APPEND CUDA_TARGET_OPTIONS
            "--std=c++17" "-Wno-deprecated-gpu-targets")
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/FS") # prevent race on PDB
        if (CUDA_CRT_LINKAGE STREQUAL "dynamic")
            if (CMAKE_BUILD_TYPE MATCHES Debug)
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MDd" "--debug" "-O0")
            else()
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MD" "-O2")
            endif()
        elseif (CUDA_CRT_LINKAGE STREQUAL "static")
            if(CMAKE_BUILD_TYPE MATCHES Debug)
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MTd" "--debug" "-O0")
            else()
                list(APPEND CUDA_TARGET_OPTIONS "-Xcompiler" "/MT" "-O2")
            endif()
        endif()
        list(APPEND DEFAULT_COMPILE_OPTIONS
                /wd4515 # -> disable warning: [Thrust issue] 'detail': namespace uses itself
                /wd4996 # -> disable warning: [Thrust issue] 'strerror': This function or variable may be unsafe.
                /wd4800 # -> disable warning: [Thrust issue] forcing value to bool 'true' or 'false' (performance warning)
                )
    endif()
    message(STATUS "Using CUDA: ${CUDA_VERSION}")
    message(STATUS "  CRT: ${CUDA_CRT_LINKAGE}")
    message(STATUS "  options: ${CUDA_TARGET_OPTIONS}")
    message(STATUS "  nvcc flags: ${CUDA_NVCC_FLAGS}")
    # note: This is for CI build. It fails to find this library so we are mentioning it here
    if(EXISTS ${CUDA_cudadevrt_LIBRARY})
        message(STATUS "  cudadevrt: ${CUDA_cudadevrt_LIBRARY}")
        get_filename_component(CUDA_LIB_DIR ${CUDA_cudadevrt_LIBRARY} DIRECTORY)
        link_directories(${CUDA_LIB_DIR})
        message(STATUS "  lib: ${CUDA_LIB_DIR}")
    endif()
elseif ()
    return()
endif ()

add_executable(snow main.cpp)
target_include_directories(
        snow
        PUBLIC
        ../../kasumi/backends/deps/igl
)