cmake_minimum_required(VERSION 3.16)
project(radar_tests CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_cfar test_cfar.cpp)
target_link_libraries(test_cfar PRIVATE radar_core gtest gtest_main)
add_test(NAME test_cfar COMMAND test_cfar)

add_executable(test_tracker test_tracker.cpp)
target_link_libraries(test_tracker PRIVATE radar_core gtest gtest_main)
add_test(NAME test_tracker COMMAND test_tracker)
