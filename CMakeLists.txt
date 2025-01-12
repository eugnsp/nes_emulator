cmake_minimum_required(VERSION 3.13)
project(nes_emulator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -fmax-errors=5")

file(GLOB SRC_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(tests "${SRC_FILES}" "${SRC_TEST_FILES}")
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
