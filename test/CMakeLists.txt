project(liblsoda-test)

add_executable(
    test-lsoda
    ${CMAKE_CURRENT_SOURCE_DIR}/test.c
)
target_link_libraries(
    test-lsoda
    PRIVATE
    lsoda::lsoda
)
add_test(
    NAME test-lsoda
    COMMAND $<TARGET_FILE:test-lsoda>
)