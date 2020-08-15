cmake_minimum_required(VERSION 3.18)
project(tests NONE)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(tests main.cc rc6.cc binops.cc)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
