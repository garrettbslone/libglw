cmake_minimum_required(VERSION 3.20)
project(glw)

set(CMAKE_CXX_STANDARD 17)

set(TARGET_DIRS "math" "util")

FILE(GLOB MATH_SRCS math/*.[ch]pp)
FILE(GLOB UTIL_SRCS util/*.[ch]pp)

add_library(${PROJECT_NAME} SHARED glw.cpp glw.hpp ${MATH_SRCS} ${UTIL_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${TARGET_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${TARGET_DIRS})

add_subdirectory(t)
