find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})


add_library(libosarracino STATIC Board.cpp Board.h Connector.cpp Connector.h MoveCalculator.cpp MoveCalculator.h Evaluator.cpp Evaluator.h Minimax.cpp Minimax.h)
target_link_libraries(libosarracino ${JSONCPP_LIBRARIES})

target_include_directories (libosarracino PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
