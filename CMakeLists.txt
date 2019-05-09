cmake_minimum_required(VERSION 3.7)
project(osarracino)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

find_package(GTest)

add_subdirectory(lib)
add_subdirectory(cli)

if (GTEST_FOUND)
    add_subdirectory(test)
endif()