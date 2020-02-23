cmake_minimum_required(VERSION 3.13)
project(MIMAS C)

add_library(mimas
    "${CMAKE_CURRENT_SOURCE_DIR}/public/mimas/mimas_gl.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/public/mimas/mimas_vk.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/public/mimas/mimas.h")
target_include_directories(mimas
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
)
add_subdirectory(private)
