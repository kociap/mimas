cmake_minimum_required(VERSION 3.13)
project(MIMAS C)

option(MIMAS_BUILD_WINDOWS "Build mimas for Windows" OFF)
option(MIMAS_BUILD_WAYLAND "Build mimas for Wayland (Linux)" OFF)

add_library(mimas
  "${CMAKE_CURRENT_SOURCE_DIR}/public/mimas/mimas_gl.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/public/mimas/mimas_vk.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/public/mimas/mimas.h")
target_include_directories(mimas
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
)
add_subdirectory(private)
