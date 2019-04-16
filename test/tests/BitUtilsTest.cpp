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

class BitUtilsTest : public ::testing::Test {
protected:

    virtual void SetUp() {

    }
    virtual void TearDown() {

    }
};

TEST_F(BitUtilsTest, test_high_moves_no_limit) {
    uint16_t row = 0b0000'0000'0000'1000;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 5);
}

TEST_F(BitUtilsTest, test_high_moves_limit) {
    uint16_t row = 0b0000'0000'0010'1000;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 1);
}

TEST_F(BitUtilsTest, test_high_moves_limit_2) {
    uint16_t row = 0b0000'0000'1100'1000;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 2);
}

TEST_F(BitUtilsTest, test_high_moves_limit_3) {
    uint16_t row = 0b0000'0000'1100'1110;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 2);
}

TEST_F(BitUtilsTest, test_high_moves_limit_4) {
    uint16_t row = 0b0000'0000'1100'1010;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 2);
}

TEST_F(BitUtilsTest, test_high_moves_all_moves) {
    uint16_t row = 0b0000'0000'0000'0001;
    EXPECT_EQ(BitUtils::get_high_moves(row, 0), 8);
}

TEST_F(BitUtilsTest, test_high_moves_last_limit) {
    uint16_t row = 0b0000'0001'0000'0001;
    EXPECT_EQ(BitUtils::get_high_moves(row, 0), 7);
}

TEST_F(BitUtilsTest, test_low_moves_no_limit) {
    uint16_t row = 0b0000'0000'0000'1000;
    EXPECT_EQ(BitUtils::get_low_moves(row, 3), 3);
}

TEST_F(BitUtilsTest, test_low_moves_limit) {
    uint16_t row = 0b0000'0000'0000'1010;
    EXPECT_EQ(BitUtils::get_low_moves(row, 3), 1);
}

TEST_F(BitUtilsTest, test_low_moves_limit_2) {
    uint16_t row = 0b0000'0000'0100'1010;
    EXPECT_EQ(BitUtils::get_low_moves(row, 3), 1);
}

TEST_F(BitUtilsTest, test_low_moves_all_moves) {
    uint16_t row = 0b0000'0001'0000'0000;
    EXPECT_EQ(BitUtils::get_low_moves(row, 8), 8);
}

TEST_F(BitUtilsTest, test_low_moves_last_limit) {
    uint16_t row = 0b0000'0001'0000'0001;
    EXPECT_EQ(BitUtils::get_low_moves(row, 8), 7);
}
//
//TEST(Test_WhiteEvaluator, test_white_evaluator_double_side_winning_move) {
//    Board b;
//    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"KING\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
//    WhiteEvaluator eval;
//    int result = eval.evaluate(b);
//    EXPECT_EQ(result, WHITE_EVALUATOR_MAX_DEPTH*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER*2);
//}
//
//TEST(Test_WhiteEvaluator, test_white_evaluator_single_side_winning_move) {
//    Board b;
//    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"KING\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
//    WhiteEvaluator eval;
//    int result = eval.evaluate(b);
//    EXPECT_EQ(result, WHITE_EVALUATOR_MAX_DEPTH*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER);
//}
//
//TEST(Test_WhiteEvaluator, test_white_evaluator_depth_2) {
//    Board b;
//    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"KING\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
//    WhiteEvaluator eval;
//    int result = eval.evaluate(b);
//    EXPECT_EQ(result, (WHITE_EVALUATOR_MAX_DEPTH-1)*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER);
//}
//
//TEST(Test_WhiteEvaluator, test_white_evaluator_depth_3) {
//    Board b;
//    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"KING\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
//    WhiteEvaluator eval;
//    int result = eval.evaluate(b);
//    EXPECT_EQ(result, (WHITE_EVALUATOR_MAX_DEPTH-2)*WHITE_EVALUATOR_SEARCH_WIN_MULTIPLIER);
//}
//
//TEST(Test_WhiteEvaluator, test_white_too_many_steps) {
//    Board b;
//    b.load_board("{\"board\": [[\"EMPTY\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"KING\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
//    WhiteEvaluator eval;
//    int result = eval.evaluate(b);
//    EXPECT_EQ(result, 9*WHITE_EVALUATOR_BLACK_PAWN_MULTIPLIER+12*WHITE_EVALUATOR_FREE_WINPOINT_MULTIPLIER);
//}
//
//TEST(Test_WhiteEvaluator, test_white_initial_game) {
//    Board b;
//    b.load_board("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
//    WhiteEvaluator eval;
//    int result = eval.evaluate(b);
//    EXPECT_EQ(result, 16*WHITE_EVALUATOR_BLACK_PAWN_MULTIPLIER+8*WHITE_EVALUATOR_WHITE_PAWN_MULTIPLIER+
//                      16*WHITE_EVALUATOR_FREE_WINPOINT_MULTIPLIER);
//}