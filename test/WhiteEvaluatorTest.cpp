//
// Created by freddy on 16/04/19.
//

#include <model/Board.h>
#include <unordered_map>
#include <functional>
#include <evaluator/Evaluator.h>
#include <evaluator/TorettoWhiteEvaluator.h>
#include <util/BitUtils.h>
#include "gtest/gtest.h"

class WhiteEvaluatorTest : public ::testing::Test {
protected:

    virtual void SetUp() {

    }
    virtual void TearDown() {

    }
};

Board get_initial_board() {
    return Board::from_path("boards/initial_board.tbbf");
}


TEST_F(WhiteEvaluatorTest, test_white_evaluator_double_side_winning_move) {
    auto b {Board::from_path("boards/test_white_evaluator_double_side_winning_move.tbbf")};
    TorettoWhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_GT(result, eval.evaluate(get_initial_board()));
}

TEST_F(WhiteEvaluatorTest, test_white_evaluator_single_side_winning_move) {
    auto b {Board::from_path("boards/test_white_evaluator_single_side_winning_move.tbbf")};
    TorettoWhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_GT(result, eval.evaluate(get_initial_board()));
}

TEST_F(WhiteEvaluatorTest, test_white_evaluator_surrounded_is_worse) {
    auto surrounded {Board::from_path("boards/test_white_evaluator_surrounded_is_worse_surrounded.tbbf")};
    auto not_surrounded {Board::from_path("boards/test_white_evaluator_surrounded_is_worse_not_surrounded.tbbf")};

    TorettoWhiteEvaluator eval;
    EXPECT_LT(eval.evaluate(surrounded), eval.evaluate(not_surrounded));
}