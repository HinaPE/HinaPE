cmake_minimum_required(VERSION 3.16)
project(HinaPE VERSION 1.1.1 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

# Load render-engine
add_subdirectory(render-engine)

# load examples
set(Examples
        sph-fluid
        pbd-cloth
        )

foreach (Example ${Examples})
    file(GLOB_RECURSE HINAPE_EXAMPLE_SPH_SRC examples/${Example}/*.cpp examples/${Example}/*.h)
    add_executable(Example-${Example} ${HINAPE_EXAMPLE_SPH_SRC} render-engine/platform/icon.rc)
    set_target_properties(Example-${Example} PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)

    target_include_directories(Example-${Example} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} physics-engine)
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

#add_subdirectory(render-engine-new)
#
#file(GLOB_RECURSE HINAPE_NEW_RENDERER_SRC examples/new-renderer/main.cpp)
#add_executable(New-Render ${HINAPE_NEW_RENDERER_SRC} render-engine/platform/icon.rc)
#set_target_properties(New-Render PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
#
#target_include_directories(New-Render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(New-Render PUBLIC HinaPE_Render_Engine_Pro)
#
#if (APPLE)
#    find_library(APPKIT_LIB AppKit)
#    target_link_libraries(New-Render PRIVATE ${APPKIT_LIB})
#endif ()
#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    target_compile_options(New-Render PUBLIC -fno-omit-frame-pointer)
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
#    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
#endif ()