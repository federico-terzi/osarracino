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

class BoardTest : public ::testing::Test {
};

TEST_F(BoardTest, test_board_equality) {
    Board b1 = Board::from_path("boards/test_board_equality_1.tbbf");
    Board b2 = Board::from_path("boards/test_board_equality_1.tbbf");
    EXPECT_EQ(b1, b2);
}

TEST_F(BoardTest, test_board_simple_white_move) {
    auto b1 = Board::from_path("boards/test_board_simple_white_move_1.tbbf");
    auto b2 = Board::from_path("boards/test_board_simple_white_move_2.tbbf");
    b2.last_move = {3,6};
    EXPECT_EQ(Board::from_board(b1, {3,3}, {3,6}), b2);
}

TEST_F(BoardTest, test_board_simple_black_move) {
    Board b1 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK\"}");
    Board b2 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    b2.last_move = {6,6};
    EXPECT_EQ(Board::from_board(b1, {5,6}, {6,6}), b2);
}

TEST_F(BoardTest, test_board_simple_eaten_pawn_white) {
    Board b1 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK4\"}");
    Board b2 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    b2.last_move = {5,3};
    EXPECT_EQ(Board::from_board(b1, {5,0}, {5,3}), b2);
}

TEST_F(BoardTest, test_board_eaten_pawn_white_with_throne) {
    Board b1 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK4\"}");
    Board b2 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    b2.last_move = {4,2};
    EXPECT_EQ(Board::from_board(b1, {4,1}, {4,2}), b2);
}

TEST_F(BoardTest, test_board_eaten_pawn_black) {
    Board b1 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    Board b2 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK\"}");
    b2.last_move = {1, 5};
    EXPECT_EQ(Board::from_board(b1, {4,5}, {1,5}), b2);
}

TEST_F(BoardTest, test_board_eaten_pawn_citadel) {
    Board b1 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK\"}");
    Board b2 = Board::from_json("{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    b2.last_move = {2,3};
    EXPECT_EQ(Board::from_board(b1, {8,3}, {2,3}), b2);
}

TEST_F(BoardTest, test_board_is_black_win_1) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_is_black_win_1.tbbf"), {5,3}, {4,3});
    EXPECT_TRUE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_is_black_win_2) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_is_black_win_2.tbbf"), {0,3}, {4,3});
    EXPECT_TRUE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_is_black_win_3) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_is_black_win_3.tbbf"), {5,3}, {5,2});
    EXPECT_FALSE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_is_black_win_4) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_is_black_win_4.tbbf"), {8,3}, {4,3});
    EXPECT_TRUE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_king_surrounded_in_throne) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_in_throne.tbbf");
    EXPECT_TRUE(b1.has_black_surrounded_throne());
}

TEST_F(BoardTest, test_board_king_surrounded_in_throne_2) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_in_throne_2.tbbf");
    EXPECT_FALSE(b1.has_black_surrounded_throne());
}

TEST_F(BoardTest, test_board_king_surrounded_by_black_or_throne) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_by_black_or_throne.tbbf");
    EXPECT_TRUE(b1.is_king_surrounded_by_black_or_throne());
}

TEST_F(BoardTest, test_board_king_surrounded_by_black_or_throne_2) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_by_black_or_throne_2.tbbf");
    EXPECT_FALSE(b1.is_king_surrounded_by_black_or_throne());
}


TEST_F(BoardTest, test_board_king_surrounded_by_black_or_throne_3) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_by_black_or_throne_3.tbbf");
    EXPECT_TRUE(b1.is_king_surrounded_by_black_or_throne());
}

TEST_F(BoardTest, test_board_king_surrounded_by_black_or_throne_4) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_by_black_or_throne_4.tbbf");
    EXPECT_FALSE(b1.is_king_surrounded_by_black_or_throne());
}


TEST_F(BoardTest, test_board_king_surrounded_by_black_or_throne_5) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_by_black_or_throne_5.tbbf");
    EXPECT_TRUE(b1.is_king_surrounded_by_black_or_throne());
}

TEST_F(BoardTest, test_board_king_surrounded_by_black_or_throne_6) {
    auto b1 = Board::from_path("boards/test_board_king_surrounded_by_black_or_throne_6.tbbf");
    EXPECT_TRUE(b1.is_king_surrounded_by_black_or_throne());
}


TEST_F(BoardTest, test_board_black_win_surround_in_throne) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_black_win_surround_in_throne.tbbf"), {3,3}, {4,3});
    EXPECT_TRUE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_black_win_surround_in_throne_2) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_black_win_surround_in_throne_2.tbbf"), {3,6}, {3,4});
    EXPECT_TRUE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_black_win_surround_in_throne_3) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_black_win_surround_in_throne_3.tbbf"), {7,4}, {5,4});
    EXPECT_TRUE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_black_win_king_adiacent_throne_1) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_black_win_king_adiacent_throne_1.tbbf"), {4,3}, {5,3});
    EXPECT_TRUE(b1.is_black_win());
}

TEST_F(BoardTest, test_board_black_win_king_adiacent_throne_2) {
    auto b1 = Board::from_board(Board::from_path("boards/test_board_black_win_king_adiacent_throne_2.tbbf"), {4,7}, {4,6});
    EXPECT_TRUE(b1.is_black_win());
}