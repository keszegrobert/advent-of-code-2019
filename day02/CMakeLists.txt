cmake_minimum_required(VERSION 3.0.0)
project(day02 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(day02 main.cpp)
target_link_libraries(day02 stdc++)
