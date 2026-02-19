cmake_minimum_required(VERSION 3.15)
project(nes_emulator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-flto=jobserver -g -Wall -Wextra -Wno-conversion -Wpedantic -Wno-interference-size -march=native)
add_compile_options($<$<CONFIG:Debug>:-ggdb3>)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fmax-errors=3 -fconcepts-diagnostics-depth=5)
endif()

add_subdirectory(emu)
add_subdirectory(app_test)
add_subdirectory(app_basic)
add_subdirectory(app_nes)
