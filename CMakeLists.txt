cmake_minimum_required(VERSION 3.0.0)
project(aoc2019 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include_directories(/usr/local/include)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(GTest REQUIRED)

if(NOT GTEST_FOUND)
    MESSAGE(FATAL_ERROR, "NO GTEST FOUND")
endif()

add_subdirectory(day01)
add_subdirectory(day02)
