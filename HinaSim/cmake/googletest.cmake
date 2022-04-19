if (TARGET gtest)
    return()
endif ()

include(FetchContent)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FETCHCONTENT_BASE_DIR "${MY_EXTERNAL_DIR}/fetched-content-Debug")
else ()
    set(FETCHCONTENT_BASE_DIR "${MY_EXTERNAL_DIR}/fetched-content-Release")
endif ()
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
