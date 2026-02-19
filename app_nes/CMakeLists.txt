file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h)
add_executable(app_nes ${SOURCES})

find_package(SFML COMPONENTS window graphics system audio REQUIRED)
target_link_libraries(app_nes PRIVATE emu sfml-system sfml-window sfml-graphics sfml-audio)
