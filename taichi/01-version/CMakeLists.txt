get_filename_component(EXAMPLE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
message("")
message("[[Example: ${EXAMPLE_NAME}]]")

project(${EXAMPLE_NAME})
cmake_minimum_required(VERSION 3.10)

# The examples requires C++ to compile.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# We need to set `FindTaichi.cmake` in module path to `find_package(Taichi)`.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
find_package(Taichi REQUIRED)

add_executable(${EXAMPLE_NAME} main.cpp)
target_link_libraries(${EXAMPLE_NAME} PRIVATE Taichi::Runtime)

# Copy redistributed libraries (`.so` and `.dll`) next to the executable.
add_custom_command(
    TARGET ${EXAMPLE_NAME}
    PRE_BUILD
    COMMENT "-- Copy redistributed libraries to output directory (if different): ${Taichi_REDIST_LIBRARIES}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Taichi_REDIST_LIBRARIES} "$<TARGET_FILE_DIR:${EXAMPLE_NAME}>")
