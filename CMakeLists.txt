cmake_minimum_required(VERSION 3.13)
project(osarracino)

set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

add_executable(osarracino main.cpp Board.cpp Board.h Connector.cpp Connector.h)
target_link_libraries(osarracino ${JSONCPP_LIBRARIES})