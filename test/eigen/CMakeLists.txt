project(liblsoda-test-eigen-compatability CXX)

find_package(Eigen3 REQUIRED)

add_executable(
    test-lsoda-eigen
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cc
)

target_link_libraries(
    test-lsoda-eigen
    PRIVATE
    lsoda::lsoda
    Eigen3::Eigen
)

add_test(
    NAME test-lsoda-eigen
    COMMAND $<TARGET_FILE:test-lsoda-eigen>
)