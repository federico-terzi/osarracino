//
// Created by freddy on 16/04/19.
//

#include <model/Board.h>
#include <unordered_map>
#include <functional>
#include <evaluator/Evaluator.h>
#include <evaluator/WhiteEvaluator.h>
#include <util/BitUtils.h>
#include "gtest/gtest.h"

class WhiteEvaluatorTest : public ::testing::Test {
protected:

    virtual void SetUp() {

    }
    virtual void TearDown() {

    }
};


TEST_F(WhiteEvaluatorTest, test_white_evaluator_double_side_winning_move) {
    Board b;
    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"KING\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    WhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_EQ(result, WHITE_EVALUATOR_MAX_DEPTH*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER*2);
}

TEST_F(WhiteEvaluatorTest, test_white_evaluator_single_side_winning_move) {
    Board b;
    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"KING\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    WhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_EQ(result, WHITE_EVALUATOR_MAX_DEPTH*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER);
}

TEST_F(WhiteEvaluatorTest, test_white_evaluator_depth_2) {
    Board b;
    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"KING\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    WhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_EQ(result, (WHITE_EVALUATOR_MAX_DEPTH-1)*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER);
}

TEST_F(WhiteEvaluatorTest, test_white_evaluator_depth_3) {
    Board b;
    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"KING\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    WhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_EQ(result, (WHITE_EVALUATOR_MAX_DEPTH-2)*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER);
}

TEST_F(WhiteEvaluatorTest, test_white_too_many_steps) {
    Board b;
    b.load_board("{\"board\": [[\"EMPTY\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"KING\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    WhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_EQ(result, 9*WHITE_EVALUATOR_BLACK_PAWN_MULTIPLIER+12*WHITE_EVALUATOR_FREE_WINPOINT_MULTIPLIER);
}

TEST_F(WhiteEvaluatorTest, test_white_initial_game) {
    Board b;
    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    WhiteEvaluator eval;
    int result = eval.evaluate(b);
    EXPECT_EQ(result, 16*WHITE_EVALUATOR_BLACK_PAWN_MULTIPLIER+8*WHITE_EVALUATOR_WHITE_PAWN_MULTIPLIER+
                      16*WHITE_EVALUATOR_FREE_WINPOINT_MULTIPLIER);
}