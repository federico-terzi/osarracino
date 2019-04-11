find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(osarracino_tests_run test.cpp)
target_link_libraries(osarracino_tests_run ${GTEST_LIBRARIES} pthread)