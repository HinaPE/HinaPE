cmake_minimum_required(VERSION 3.16)
project(004-xyz-viewer LANGUAGES CXX)

add_executable(004-xyz-viewer main.cpp)
set_target_properties(004-xyz-viewer PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(004-xyz-viewer PUBLIC Kasumi_renderer nfd)
