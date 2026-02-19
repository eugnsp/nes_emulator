add_executable(app_test main.cpp)
target_link_libraries(app_test PRIVATE emu)

set_property(TARGET app_test PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

add_custom_command(
    TARGET app_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/test1/6502_functional_test.bin
            ${CMAKE_CURRENT_BINARY_DIR}/6502_functional_test.bin
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/test2/nestest.nes
            ${CMAKE_CURRENT_BINARY_DIR}/nestest.nes
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/test2/nestest.log
            ${CMAKE_CURRENT_BINARY_DIR}/nestest.log
)
