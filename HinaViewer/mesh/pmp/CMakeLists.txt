file(GLOB_RECURSE PMP_SRC *.h *.cpp)
add_definitions(-DM_PI=3.14159265358)
add_library(pmp ${PMP_SRC})
target_include_directories(
        pmp
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
)
