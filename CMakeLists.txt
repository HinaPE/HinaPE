cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.14 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

set(HINAPE_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set(HINAPE_PHYSICS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/physics-engine)
set(HINAPE_RENDERER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/render-engine)

option(HinaPE_CLOTH_ENGINE "Load HinaPE Cloth Engine" ON)
option(HinaPE_RIGIDBODY_ENGINE "Load HinaPE Rigidbody Engine" ON)
option(HinaPE_FLUID_ENGINE "Load HinaPE Fluid Engine" ON)
option(HinaPE_EXAMPLES "Load HinaPE Examples" ON)

# Load engine modules
if (HinaPE_CLOTH_ENGINE)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/cloth-engine)
endif ()

if (HinaPE_RIGIDBODY_ENGINE)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/fluid-engine)
endif ()

if (HinaPE_FLUID_ENGINE)
    add_subdirectory(${HINAPE_PHYSICS_DIR}/rigidbody-engine)
endif ()

get_directory_property(HINAPE_PARENT_DIR PARENT_DIRECTORY)
if (HINAPE_PARENT_DIR)
    add_subdirectory(${HINAPE_RENDERER_DIR})
    if (HinaPE_EXAMPLES)
        set(Examples
                sph-fluid
                pbd-cloth
                )
        foreach (Example ${Examples})
            file(GLOB_RECURSE HINAPE_EXAMPLE_SPH_SRC examples/${Example}/*.cpp examples/${Example}/*.h)
            add_executable(Example-${Example} ${HINAPE_EXAMPLE_SPH_SRC} render-engine/platform/icon.rc)
            set_target_properties(Example-${Example} PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)

            target_include_directories(Example-${Example} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${HINAPE_PHYSICS_DIR} ${HINAPE_RENDERER_DIR})
            target_link_libraries(Example-${Example} PUBLIC HinaPE_Render_Engine)

            if (APPLE)
                find_library(APPKIT_LIB AppKit)
                target_link_libraries(Example-${Example} PRIVATE ${APPKIT_LIB})
            endif ()
            if (CMAKE_CXX_COMPILER_ID MATCHES "Clang") # fixed by: https://github.com/libMesh/libmesh/issues/1396
                target_compile_options(Example-${Example} PUBLIC -fno-omit-frame-pointer)
                set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
                set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
            endif ()
        endforeach ()
    endif ()
endif ()
