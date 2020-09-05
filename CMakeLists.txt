cmake_minimum_required(VERSION 3.13)
project(rc6)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_subdirectory(include)
add_subdirectory(tests)
