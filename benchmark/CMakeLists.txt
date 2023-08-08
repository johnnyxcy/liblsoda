add_executable(
    lsoda-benchmark
    ${CMAKE_CURRENT_SOURCE_DIR}/runner.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/fex.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/scipy.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

find_package(Boost REQUIRED)

target_link_libraries(
  lsoda-benchmark
    PRIVATE
    lsoda::lsoda
    Boost::boost
)
