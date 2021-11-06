cmake_minimum_required(VERSION 3.16)
project(glw)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB GRAPHICS_SRCS *.[ch]pp)
FILE(GLOB GL_SRCS opengl/*.[ch]pp)

add_library(graphics ${GRAPHICS_SRCS} ${GL_SRCS} ${CMAKE_SOURCE_DIR}/ext/glad/gl.h
        ${CMAKE_SOURCE_DIR}/util/exception.cpp ${CMAKE_SOURCE_DIR}/util/exception.cpp)

MESSAGE(${CMAKE_SOURCE_DIR})
target_include_directories(graphics PUBLIC
        ${CMAKE_SOURCE_DIR}/etx/glfw/include PUBLIC
        ${CMAKE_SOURCE_DIR}/ext/stb_image PUBLIC
        ${CMAKE_SOURCE_DIR}/etx PUBLIC)
target_link_directories(graphics PRIVATE ${CMAKE_SOURCE_DIR}/ext/glfw/src)
target_link_libraries(graphics glfw)
