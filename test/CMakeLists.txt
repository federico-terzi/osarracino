project(osarracino_tests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(osarracino_tests_run MainTestAll.cpp
        BitUtilsTest.cpp
        BoardTest.cpp
        MoveGeneratorTest.cpp BlackEvaluatorTest.cpp
        WhiteEvaluatorTest.cpp ToBeMovedEmptyTest.cpp)

target_link_libraries(osarracino_tests_run libosarracino)
target_link_libraries(osarracino_tests_run ${GTEST_LIBRARIES} pthread)

file(COPY boards DESTINATION ${CMAKE_CURRENT_BINARY_DIR})