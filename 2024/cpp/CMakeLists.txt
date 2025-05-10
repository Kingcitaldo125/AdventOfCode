cmake_minimum_required(VERSION 3.16)

project(AOC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(TUs)

file(GLOB_RECURSE TUs "main.cpp")

foreach(itm IN ${TUs})
    add_executable(${itm})
endforeach()
