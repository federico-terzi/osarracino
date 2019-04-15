#include <model/Board.h>
#include <unordered_map>
#include <functional>
#include <evaluator/Evaluator.h>
#include <evaluator/WhiteEvaluator.h>
#include <util/BitUtils.h>
#include "gtest/gtest.h"

TEST(Test_Move_Calculator, test_white_evaluator) {   // Situazione iniziale
    Board b;
    b.load_board("{\"board\": [[\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}");
    WhiteEvaluator eval;
    eval.evaluate(b);
    //EXPECT_EQ(b.is_white, 8);
}

TEST(Test_BitUtils, test_high_moves_no_limit) {
    uint16_t row = 0b0000'0000'0000'1000;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 5);
}

TEST(Test_BitUtils, test_high_moves_limit) {
    uint16_t row = 0b0000'0000'0010'1000;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 1);
}

TEST(Test_BitUtils, test_high_moves_limit_2) {
    uint16_t row = 0b0000'0000'1100'1000;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 2);
}

TEST(Test_BitUtils, test_high_moves_limit_3) {
    uint16_t row = 0b0000'0000'1100'1110;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 2);
}

TEST(Test_BitUtils, test_high_moves_limit_4) {
    uint16_t row = 0b0000'0000'1100'1010;
    EXPECT_EQ(BitUtils::get_high_moves(row, 3), 2);
}

TEST(Test_BitUtils, test_high_moves_all_moves) {
    uint16_t row = 0b0000'0000'0000'0001;
    EXPECT_EQ(BitUtils::get_high_moves(row, 0), 8);
}

TEST(Test_BitUtils, test_high_moves_last_limit) {
    uint16_t row = 0b0000'0001'0000'0001;
    EXPECT_EQ(BitUtils::get_high_moves(row, 0), 7);
}

TEST(Test_BitUtils, test_low_moves_no_limit) {
    uint16_t row = 0b0000'0000'0000'1000;
    EXPECT_EQ(BitUtils::get_low_moves(row, 3), 3);
}

TEST(Test_BitUtils, test_low_moves_limit) {
    uint16_t row = 0b0000'0000'0000'1010;
    EXPECT_EQ(BitUtils::get_low_moves(row, 3), 1);
}

TEST(Test_BitUtils, test_low_moves_limit_2) {
    uint16_t row = 0b0000'0000'0100'1010;
    EXPECT_EQ(BitUtils::get_low_moves(row, 3), 1);
}

TEST(Test_BitUtils, test_low_moves_all_moves) {
    uint16_t row = 0b0000'0001'0000'0000;
    EXPECT_EQ(BitUtils::get_low_moves(row, 8), 8);
}

TEST(Test_BitUtils, test_low_moves_last_limit) {
    uint16_t row = 0b0000'0001'0000'0001;
    EXPECT_EQ(BitUtils::get_low_moves(row, 8), 7);
}


int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}