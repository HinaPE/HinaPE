# HinaPE使用现代CMake，因此必须支持3.16及以上版本的Cmake
cmake_minimum_required(VERSION 3.16)

# 新建一个独立的Project，并指定为C++的Project（如果不指定，编译器将有概率将程序误判为C工程，导致部分文件无法编译）。
project(KasumiFromScratch LANGUAGES CXX)

# 添加一个可执行文件，编译源文件指定为main.cpp api.h api.cpp这三个文件。
add_executable(000-KasumiFromScratch main.cpp)

# HinaPE积极拥抱现代的C++，因此，我们将使用C++20标准。
set_target_properties(000-KasumiFromScratch PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)

# 将Kasumi_renderer链接进KasumiFromScratch这个Target。这样，KasumiFromScratch就可以使用Kasumi_renderer的全部功能了（包括Kasumi_renderer自己定义的头文件也会被链式法则引入到KasumiFromScratch）。
target_link_libraries(000-KasumiFromScratch PUBLIC Kasumi_renderer)

# 为了方便，我们添加一个我们自己的Asset的文件夹的宏。
target_compile_definitions(
        000-KasumiFromScratch
        PUBLIC
        -DMyAssetsDir="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)