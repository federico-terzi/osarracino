find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES} pthread)


add_library(libosarracino STATIC
        model/Board.cpp model/Board.h
        network/Connector.cpp network/Connector.h
        evaluator/Evaluator.h
        model/Position.cpp model/Position.h
        evaluator/TorettoWhiteEvaluator.cpp evaluator/TorettoWhiteEvaluator.h
        evaluator/BlackEvaluator.cpp evaluator/BlackEvaluator.h
        movegenerator/MoveGenerator.h
        movegenerator/ThetaMoveGenerator.cpp movegenerator/ThetaMoveGenerator.h
        util/BitUtils.h util/BoardUtils.cpp util/BoardUtils.h
        movegenerator/ArnoldMoveGenerator.cpp movegenerator/ArnoldMoveGenerator.h
        search/SearchEngine.h search/RamboSearchEngine.h util/Timer.h
        movegenerator/HeisenbergMoveGenerator.cpp movegenerator/HeisenbergMoveGenerator.h
        evaluator/ReverseTorettoBlackEvaluator.cpp evaluator/ReverseTorettoBlackEvaluator.h
        model/ZobristHash.cpp model/ZobristHash.h
        model/DTranspositionTable.cpp model/DTranspositionTable.h
        search/CollisionSearchEngine.h search/CarloSearchEngine.h
        search/ThanosSearchEngine.h
        search/FSMSearchEngine.h movegenerator/MadaraMoveGenerator.cpp movegenerator/MadaraMoveGenerator.h)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

target_link_libraries(libosarracino ${JSONCPP_LIBRARIES} pthread)

target_include_directories (libosarracino PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
