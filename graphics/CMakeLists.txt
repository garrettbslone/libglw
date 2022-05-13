cmake_minimum_required(VERSION 3.16)
project(glw)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB GRAPHICS_SRCS *.[ch]pp)
FILE(GLOB GL_SRCS opengl/*.[ch]pp)

add_library(graphics
        ${CMAKE_SOURCE_DIR}/ext/glad/gl.h
        ${CMAKE_SOURCE_DIR}/util/exception.cpp
        ${CMAKE_SOURCE_DIR}/util/exception.cpp)

if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
    set(VULKAN_FOUND "True")
else()
    find_package(Vulkan)
    message(STATUS "Found Vulkan: ${VULKAN_FOUND}")
endif()

if (NOT VULKAN_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library! Compiling with only OpenGL support!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
    message(STATUS "Headers at: ${Vulkan_INCLUDE_DIRS}")
    target_compile_definitions(graphics PUBLIC HAVE_VULKAN=1)
    FILE(GLOB VK_SRCS vulkan/*.[ch]pp)
    target_sources(graphics PUBLIC ${VK_SRCS} ${CMAKE_SOURCE_DIR}/ext/glad/vulkan.h)
    target_include_directories(graphics PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(graphics PUBLIC ${Vulkan_LIBRARIES})
endif()

target_sources(graphics PUBLIC ${GRAPHICS_SRCS} ${GL_SRCS})
target_include_directories(graphics PUBLIC ${CMAKE_SOURCE_DIR}/etx/glfw/include
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/stb_image
        PUBLIC ${CMAKE_SOURCE_DIR}/etx
        PUBLIC ${CMAKE_SOURCE_DIR}/ext/glm)

target_link_directories(graphics PRIVATE ${CMAKE_SOURCE_DIR}/ext/glfw/src)
target_link_libraries(graphics PRIVATE glfw PUBLIC input)
