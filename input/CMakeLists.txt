cmake_minimum_required(VERSION 3.16)
project(glw)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB INPUT_SRCS *.[ch]pp)

add_library(input ${INPUT_SRCS} ${CMAKE_SOURCE_DIR}/util/exception.cpp ${CMAKE_SOURCE_DIR}/util/exception.cpp key_codes.hpp)

target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../etx/glfw/include PUBLIC)
target_link_directories(input PRIVATE ${CMAKE_SOURCE_DIR}/ext/glfw/src)
target_link_libraries(input glfw)
