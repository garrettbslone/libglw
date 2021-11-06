cmake_minimum_required(VERSION 3.16)
project(glw)

set(CMAKE_CXX_STANDARD 17)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()

add_subdirectory(ext/glfw)
add_subdirectory(graphics)

set(TARGET_DIRS "math" "util" "graphics")

FILE(GLOB MATH_SRCS math/*.[ch]pp)
FILE(GLOB UTIL_SRCS util/*.[ch]pp)

add_library(${PROJECT_NAME} STATIC glw.cpp glw.hpp ${MATH_SRCS} ${UTIL_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${TARGET_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${TARGET_DIRS})
target_link_libraries(${PROJECT_NAME} graphics)

add_subdirectory(t)
