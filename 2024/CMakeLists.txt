cmake_minimum_required(VERSION 3.16)
project(advent)

set(CMAKE_CXX_STANDARD 20)

add_executable(advent main.cpp
        day1/day1.cpp
        day1/day1.h
        utils/file_reader.h
        utils/print_vector.h
        utils/numbers_in_string.h
        day2/day2.h
        day2/day2.cpp
        day3/day3.h
        day4/day4.h
        day5/day5.h
        day6/day6.h)
