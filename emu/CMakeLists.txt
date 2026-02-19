file(GLOB_RECURSE EMU_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)

add_library(emu STATIC ${EMU_SOURCES})
target_include_directories(emu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
