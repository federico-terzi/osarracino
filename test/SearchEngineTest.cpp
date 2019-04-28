//
// Created by freddy on 28/04/19.
//
#include <gtest/gtest.h>
#include <search/RamboSearchEngine.h>
#include <evaluator/ReverseTorettoBlackEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>

template <typename T>
class SearchEngineTest : public ::testing::Test {
public:
    using SearchEngineType = T;
};

typedef ::testing::Types<RamboSearchEngine> SearchEngineTypes;
TYPED_TEST_CASE(SearchEngineTest, SearchEngineTypes);

//TYPED_TEST(SearchEngineTest, test_black_winning_move) {
//    using SearchEngineType  = typename TestFixture::SearchEngineType ;
//    auto b {Board::from_path("boards/test_black_winning_move.tbbf")};
//    RamboSearchEngine engine;
//    ReverseTorettoBlackEvaluator eval;
//    ArnoldMoveGenerator move;
//    auto best_move = engine.__make_decision_internal(b, eval, move);
//    auto target_move = Move{Position{0,6}, Position{2,6}};
//    EXPECT_EQ(best_move, target_move);
//}