find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE BASIC_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_executable(app_basic ${BASIC_SOURCES})
target_link_libraries(app_basic PRIVATE emu ${CURSES_LIBRARIES})

add_custom_command(
    TARGET app_basic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/microtan.bin ${CMAKE_CURRENT_BINARY_DIR}/microtan.bin
)
