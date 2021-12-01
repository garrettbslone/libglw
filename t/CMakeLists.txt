cmake_minimum_required(VERSION 3.20)
project(glw)

set(CMAKE_CXX_STANDARD 17)

set(TEST_SRCS "graphics/graphics" "core/app")
set(TEST_BINS "graphics" "app")

foreach(BIN SRC IN ZIP_LISTS TEST_BINS TEST_SRCS)
    add_executable(${BIN}-t ${SRC}.cpp include/test_res.hpp)

    target_link_directories(${BIN}-t  PUBLIC ${CMAKE_SOURCE_DIR})
    target_include_directories(${BIN}-t  PUBLIC ${CMAKE_SOURCE_DIR})
    target_link_libraries(${BIN}-t PUBLIC glw)
endforeach()
