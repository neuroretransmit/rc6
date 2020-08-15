cmake_minimum_required(VERSION 3.18)
project(rc6)

set(CMAKE_CXX_STANDARD 17)
include_directories(src)
add_subdirectory(src)
add_subdirectory(tests)
