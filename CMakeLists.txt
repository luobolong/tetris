cmake_minimum_required(VERSION 3.28)
project(Tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 源码位置
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
include_directories(src)

# Fetch SFML 3
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# 添加可执行文件
add_executable(tetris ${SOURCES})
add_custom_command(TARGET tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:tetris>/res)

# 设置特性与链接
target_compile_features(tetris PRIVATE cxx_std_20)
target_link_libraries(tetris PRIVATE SFML::Graphics)
