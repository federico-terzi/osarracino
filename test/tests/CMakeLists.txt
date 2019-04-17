find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(osarracino_tests_run MainTestAll.cpp
        BitUtilsTest.cpp
        MoveGeneratorTest.cpp
        WhiteEvaluatorTest.cpp ToBeMovedEmptyTest.cpp)

target_link_libraries(osarracino_tests_run libosarracino)
target_link_libraries(osarracino_tests_run ${GTEST_LIBRARIES} pthread)