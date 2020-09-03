cmake_minimum_required(VERSION 3.18)
project(tests NONE)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
add_compile_definitions(DEBUG)
add_executable(tests main.cc rc6.cc binops.cc ../src/binops.cc mode/aead.cc mode/fieldelement64.cc mode/polyval.cc mode/modes.cc)
target_link_libraries(tests ${GTEST_LIBRARIES} gmock gmock_main pthread)
