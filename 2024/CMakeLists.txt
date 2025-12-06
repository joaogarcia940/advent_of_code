cmake_minimum_required(VERSION 3.22)

# Use clang-19 with libc++ for full C++23 support
set(CMAKE_CXX_COMPILER clang++-19)

project(2024 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-std=c++2c -stdlib=libc++)
add_link_options(-stdlib=libc++)

add_executable(day1 day1/day1.cpp)
add_executable(day2 day2/day2.cpp)
add_executable(day3 day3/day3.cpp)
add_executable(day4 day4/day4.cpp)
add_executable(day5 day5/day5.cpp)
add_executable(day6 day6/day6.cpp)
add_executable(day7 day7/day7.cpp)
add_executable(day8 day8/day8.cpp)
add_executable(day9 day9/day9.cpp)
