cmake_minimum_required(VERSION 3.22)

# Use clang for C++26 support
set(CMAKE_CXX_COMPILER clang++-19)

project(2024 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

# enable c++26 experimental - use libstdc++ instead of libc++ for regex support
add_compile_options(-std=c++2c)
# Note: not using -stdlib=libc++ to maintain regex support with libstdc++

add_executable(day1 day1/day1.cpp)
add_executable(day2 day2/day2.cpp)
add_executable(day3 day3/day3.cpp)
add_executable(day4 day4/day4.cpp)
add_executable(day5 day5/day5.cpp)
add_executable(day6 day6/day6.cpp)
add_executable(day7 day7/day7.cpp)
add_executable(day8 day8/day8.cpp)
