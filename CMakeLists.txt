cmake_minimum_required(VERSION 3.12)

project(liblsoda VERSION 0.2.0)

set(LSODA_VERSION 0.2.0)

# compiler
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

# Options
option(BUILD_TESTING "Build test" ON)
option(BUILD_BENCHMARK "Build benchmark" ON)

# Configuration.
find_package(Threads REQUIRED)

# include path
set(_LSODA_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
# src files
set(
    _LSODA_SRC
    src/cfode.c
    src/cfode_static.c
    src/common.c
    src/corfailure.c
    src/correction.c
    src/daxpy.c
    src/ddot.c
    src/dgefa.c
    src/dgesl.c
    src/dscal.c
    src/fnorm.c
    src/idamax.c
    src/intdy.c
    src/lsoda.c
    src/methodswitch.c
    src/orderswitch.c
    src/prja.c
    src/scaleh.c
    src/solsy.c
    src/stoda.c
    src/vmnorm.c
    src/strdup_printf.c
)

option(BUILD_STATIC_LIBS "Build lsoda as a static library" OFF)
option(BUILD_SHARED_LIBS "Build lsoda as a shared library" OFF)
option(USE_OPENMP "Build lsoda with OpenMP support" ON)

if (NOT BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
    set(BUILD_STATIC_LIBS ON)
elseif (BUILD_SHARED_LIBS AND BUILD_STATIC_LIBS)
    message(FATAL_ERROR "BUILD_SHARED_LIBS and BUILD_STATIC_LIBS cannot be both ON")
endif()

if (USE_OPENMP)
    find_package(OpenMP REQUIRED)
    if (OPENMP_FOUND)
        message(STATUS "[lsoda] Building with OpenMP support")
        add_compile_definitions(USE_OPENMP)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -ffast-math")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

# lib
if (BUILD_STATIC_LIBS)
    message(STATUS "[lsoda] Building static library")
    add_library(lsoda STATIC ${_LSODA_SRC})
elseif (BUILD_SHARED_LIBS)
    message(STATUS "[lsoda] Building shared library")
    add_library(lsoda SHARED ${_LSODA_SRC})
endif()

target_include_directories(
    lsoda
    PRIVATE
    $<BUILD_INTERFACE:${_LSODA_INCLUDE_DIR}>
)
set(LSODA_LIBRARIES lsoda)

# interface
add_library(lsoda::lsoda ALIAS lsoda)
set_property(
    TARGET 
    lsoda 
    PROPERTY 
    INTERFACE_INCLUDE_DIRECTORIES 
    $<BUILD_INTERFACE:${_LSODA_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/lsoda>
)
set_property(
    TARGET 
    lsoda
    PROPERTY 
    INTERFACE_LINK_LIBRARIES
    lsoda
)
unset(_LSODA_INCLUDE_DIR)

# Tests and benchmark
if (BUILD_TESTING)
    message(STATUS "[lsoda] Building test")
    add_subdirectory(test)
endif()

if (BUILD_BENCHMARK)
    message(STATUS "[lsoda] Building benchmark")
    add_subdirectory(benchmark)
endif()

# 将配置文件中的变量设置为实际值
configure_file(lsoda-config.cmake.in lsoda-config.cmake @ONLY)

# export lsoda
export(TARGETS lsoda NAMESPACE lsoda:: FILE lsodaTargets.cmake)

# install config
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lsoda-config.cmake"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/lsoda")

# install target
install(EXPORT lsodaTargets
        NAMESPACE lsoda::
        DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/lsoda")

# install header
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include
        DESTINATION "${CMAKE_INSTALL_PREFIX}"
        FILES_MATCHING PATTERN "*.h")

# install libraries
install(TARGETS lsoda
        EXPORT lsodaTargets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        INCLUDES DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
