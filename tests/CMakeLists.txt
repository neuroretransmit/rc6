cmake_minimum_required(VERSION 3.13)
project(tests NONE)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
add_compile_definitions(DEBUG)
add_executable(tests main.cc rc6.cc mode/aead.cc mode/aead/fieldelement64.cc mode/aead/polyval.cc mode/ctr.cc mode/ecb.cc)
target_link_libraries(tests ${GTEST_LIBRARIES} gmock gmock_main pthread)
