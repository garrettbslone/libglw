cmake_minimum_required(VERSION 3.20)
project(glw)

set(CMAKE_CXX_STANDARD 17)

set(SAMPLE_SRCS "game.cpp" "wormy.cpp" "wormy.hpp")

add_executable(game ${SAMPLE_SRCS})

target_link_directories(game PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(game PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(game PUBLIC glw)
