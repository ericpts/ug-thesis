project(thesis)
cmake_minimum_required(VERSION 2.8)
add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(thesis
    src/main.cpp
    src/types.h)
target_link_libraries(thesis ${CONAN_LIBS})
