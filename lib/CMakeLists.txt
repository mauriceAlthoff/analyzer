cmake_minimum_required(VERSION 3.16.3)

include(../build/conanbuildinfo.cmake)

conan_basic_setup()

set(BINARY ${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE LIB_SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${LIB_SOURCES})

add_library(${BINARY} STATIC ${LIB_SOURCES})
