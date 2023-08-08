project(liblsoda-test-boost CXX)

find_package(Boost REQUIRED PATHS /opt/homebrew/Cellar/boost/1.81.0_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    test-lsoda-boost
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cc
)

target_link_libraries(
    test-lsoda-boost
    PRIVATE
    lsoda::lsoda
    Boost::boost
)

add_test(
    NAME test-lsoda-boost
    COMMAND $<TARGET_FILE:test-lsoda-boost>
)