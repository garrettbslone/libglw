cmake_minimum_required(VERSION 3.20)
project(glw)

set(CMAKE_CXX_STANDARD 17)

add_executable(vec3 math/vec.cpp include/test_res.hpp)

target_link_directories(vec3 PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(vec3 PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(vec3 PUBLIC glw)
