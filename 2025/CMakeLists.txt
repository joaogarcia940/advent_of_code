cmake_minimum_required(VERSION 3.22)

# Use clang for C++26 support
set(CMAKE_CXX_COMPILER clang++-19)

project(2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# enable c++26 experimental and use libc++
add_compile_options(-std=c++2c -stdlib=libc++)
add_link_options(-stdlib=libc++)

add_executable(day1 day1/day1.cpp)
add_executable(day2 day2/day2.cpp)
add_executable(day3 day3/day3.cpp)
add_executable(day4 day4/day4.cpp)
add_executable(day5 day5/day5.cpp)
add_executable(day6 day6/day6.cpp)
add_executable(day7 day7/day7.cpp)

enable_testing()

# Build GTest from source with libc++
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(day1_test day1/test.cpp)
target_link_libraries(day1_test PRIVATE gtest_main)
target_include_directories(day1_test PRIVATE day1)

add_executable(day2_test day2/test.cpp)
target_link_libraries(day2_test PRIVATE gtest_main)
target_include_directories(day2_test PRIVATE day2)

add_executable(day3_test day3/test.cpp)
target_link_libraries(day3_test PRIVATE gtest_main)
target_include_directories(day3_test PRIVATE day3)

add_executable(day4_test day4/test.cpp)
target_link_libraries(day4_test PRIVATE gtest_main)
target_include_directories(day4_test PRIVATE day4)

add_executable(day5_test day5/test.cpp)
target_link_libraries(day5_test PRIVATE gtest_main)
target_include_directories(day5_test PRIVATE day5)

add_executable(day6_test day6/test.cpp)
target_link_libraries(day6_test PRIVATE gtest_main)
target_include_directories(day6_test PRIVATE day6)

add_executable(day7_test day7/test.cpp)
target_link_libraries(day7_test PRIVATE gtest_main)
target_include_directories(day7_test PRIVATE day7)

include(GoogleTest)
gtest_discover_tests(day1_test)
gtest_discover_tests(day2_test)
gtest_discover_tests(day3_test)
gtest_discover_tests(day4_test)
gtest_discover_tests(day5_test)
gtest_discover_tests(day6_test)
gtest_discover_tests(day7_test)
