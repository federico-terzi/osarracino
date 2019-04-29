//
// Created by max on 24/04/19.
//

#include <gtest/gtest.h>
#include <model/Board.h>
#include <model/ZobristHash.h>

class ZobristHashTest : public ::testing::Test{




};

TEST_F(ZobristHashTest, Test_Hash_Initial_Board) {
    Board b = Board::from_path("boards/initial_board.tbbf");
    ZobristHash hasher;
    auto result = hasher.hash(b);
    EXPECT_GE(result, 0);
}