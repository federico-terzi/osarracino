//
// Created by max on 24/04/19.
//

#include <gtest/gtest.h>
#include <model/Board.h>
#include <model/ZobristHash.h>
#include <movegenerator/MoveGenerator.h>

class ZobristHashTest : public ::testing::Test{
public:
ZobristHash zobrist;
    virtual void SetUp() {

    }
    virtual void TearDown() {

    }

};

TEST_F(ZobristHashTest, Test_Hash_Initial_Board) {
    Board b = Board::from_path("boards/initial_board.tbbf");
    Board b1 = Board::from_path("boards/initial_board.tbbf");
    auto hash_1 = zobrist.hash(b);
    auto hash_2 = zobrist.hash(b1);
    EXPECT_EQ(hash_1, hash_2);
}

TEST_F(ZobristHashTest, Test_Same_Hash_After_Some_Moves) {
    Board b = Board::from_path("boards/initial_board.tbbf");
    Board b1 = Board::from_path("boards/initial_board.tbbf");
    Board after = Board::from_board(b, {5,4}, {5,3});
    Board after1 = Board::from_board(b1, {5,4}, {5,3});

    EXPECT_NE(zobrist.hash((b)), zobrist.hash(after));
    EXPECT_EQ(zobrist.hash(after), zobrist.hash(after1));

}
