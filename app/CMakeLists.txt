cmake_minimum_required(VERSION 3.16.3)

include(../build/conanbuildinfo.cmake)

conan_basic_setup()

set(BINARY ${CMAKE_PROJECT_NAME}_run)

file(GLOB_RECURSE APP_SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${APP_SOURCES})

add_executable(${BINARY} ${APP_SOURCES})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib ${CONAN_LIBS})
