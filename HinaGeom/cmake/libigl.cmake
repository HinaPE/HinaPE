include(FetchContent)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FETCHCONTENT_BASE_DIR "${MY_EXTERNAL_DIR}/fetched-content-Debug")
else ()
    set(FETCHCONTENT_BASE_DIR "${MY_EXTERNAL_DIR}/fetched-content-Release")
endif ()
include_directories(${FETCHCONTENT_BASE_DIR}/eigen-src)

if (TARGET igl::core)
    return()
endif ()
FetchContent_Declare(
        libigl
        GIT_REPOSITORY https://github.com/libigl/libigl.git
        GIT_TAG v2.4.0
)
FetchContent_MakeAvailable(libigl)
