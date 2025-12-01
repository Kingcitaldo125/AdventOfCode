cmake_minimum_required(VERSION 3.16)

project(AOC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(TUs)

file(GLOB_RECURSE TUs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "main.cpp")

foreach(itm IN ITEMS ${TUs})
    set(item_name "${itm}")
    string(REGEX REPLACE "/.+$" "" item_name ${item_name})
    add_executable(${item_name} ${itm})
endforeach()
