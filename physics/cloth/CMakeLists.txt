cmake_minimum_required(VERSION 3.24)
project(HinaPE_Cloth VERSION 2.0.0 DESCRIPTION "HinaPE is not a Physics Engine" LANGUAGES CXX)

if (NOT TARGET Kasumi_Render3D)
    message(FATAL_ERROR "Kasumi_Render3D is not found")
    return()
endif ()

add_library(HinaPE_Cloth STATIC)
file(GLOB headers objects/*.h pbd/*.h)
file(GLOB sources objects/*.cpp pbd/*.cpp)
target_sources(HinaPE_Cloth
        PRIVATE
        ${sources}

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../
        FILES
        ${headers}
        )
set_target_properties(HinaPE_Cloth PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_include_directories(HinaPE_Cloth PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(HinaPE_Cloth PUBLIC Kasumi_Render3D)
target_compile_definitions(
        HinaPE_Cloth
        PUBLIC
        -DMyAssetDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)

#IF (WIN32)
#    add_subdirectory(nvcloth)
#ENDIF ()

## APP EXECUTABLE
#add_executable(HinaPE_Cloth_App main.cpp)
#set_target_properties(HinaPE_Cloth_App PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
#target_include_directories(HinaPE_Cloth_App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(HinaPE_Cloth_App PUBLIC HinaPE_Cloth)