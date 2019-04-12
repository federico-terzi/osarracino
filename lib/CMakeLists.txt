find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})


add_library(libosarracino STATIC Board.cpp Board.h Connector.cpp Connector.h BitMaskHandler.cpp BitMaskHandler.h Evaluator.cpp Evaluator.h)
target_link_libraries(libosarracino ${JSONCPP_LIBRARIES})

target_include_directories (libosarracino PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
