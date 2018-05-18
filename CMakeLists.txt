project(thesis)
cmake_minimum_required (VERSION 3.1)
include(ExternalProject)

##################################
# Register the thesis binary.

add_definitions("-std=c++17 -Wall")
add_executable(thesis
    src/main.cpp
    src/types.h
    src/types.cpp
    src/classreader.h
    src/classreader.cpp
    src/classwriter.h
    src/classwriter.cpp
    src/classfile.h
    src/classfile.cpp
    src/bytesparser.h
    src/bytesparser.cpp
    src/util.h
    src/util.cpp
    extern/CLI11.hpp
)
target_include_directories(thesis PUBLIC "extern/")

##################################
# Download and install GoogleTest

ExternalProject_Add(gtest
  # Comment above line, and uncomment line below to use subversion.
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG 08d5b1f33af8c18785fb8ca02792b5fac81e248f

  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gtest source_dir binary_dir)

################
# Define a test
add_executable(registerer_test
    test/src/test_classfile.cpp
    src/types.h
    src/types.cpp
    src/classreader.h
    src/classreader.cpp
    src/classwriter.h
    src/classwriter.cpp
    src/classfile.h
    src/classfile.cpp
    src/bytesparser.h
    src/bytesparser.cpp
    src/util.h
    src/util.cpp
)

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

add_dependencies(registerer_test gtest)
target_include_directories(registerer_test
    PUBLIC "${source_dir}/googletest/include"
    PUBLIC "${PROJECT_SOURCE_DIR}/src"
)
target_link_libraries(registerer_test
    ${binary_dir}/googlemock/gtest/libgtest.a
    ${binary_dir}/googlemock/gtest/libgtest_main.a
    pthread
    stdc++fs
)

##################################
# Just make the test runnable with
#   $ make test

enable_testing()
add_test(NAME    registerer_test
         COMMAND registerer_test)

