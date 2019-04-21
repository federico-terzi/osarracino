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

TEST_F(BitUtilsTest, test_get_surrounded_simple_double) {
    uint16_t row = 0b0000'0001'0001'1101;
    EXPECT_EQ(BitUtils::get_surrounded(row, 3), 2);
}

TEST_F(BitUtilsTest, test_get_surrounded_simple_single) {
    uint16_t row = 0b0000'0001'0001'1001;
    EXPECT_EQ(BitUtils::get_surrounded(row, 3), 1);
}

TEST_F(BitUtilsTest, test_get_surrounded_simple_free) {
    uint16_t row = 0b0000'0001'0000'1001;
    EXPECT_EQ(BitUtils::get_surrounded(row, 3), 0);
}

TEST_F(BitUtilsTest, test_get_surrounded_right_blocked) {
    uint16_t row = 0b0000'0001'0000'0011;
    EXPECT_EQ(BitUtils::get_surrounded(row, 0), 1);
}

TEST_F(BitUtilsTest, test_get_surrounded_right_free) {
    uint16_t row = 0b0000'0001'0000'0001;
    EXPECT_EQ(BitUtils::get_surrounded(row, 0), 0);
}

TEST_F(BitUtilsTest, test_get_surrounded_left_blocked) {
    uint16_t row = 0b0000'0001'1000'0001;
    EXPECT_EQ(BitUtils::get_surrounded(row, 8), 1);
}

TEST_F(BitUtilsTest, test_get_surrounded_left_free) {
    uint16_t row = 0b0000'0001'0000'0001;
    EXPECT_EQ(BitUtils::get_surrounded(row, 8), 0);
}

TEST_F(BitUtilsTest, test_get_surrounded_left_double) {
    uint16_t row = 0b0000'0001'1100'0001;
    EXPECT_EQ(BitUtils::get_surrounded(row, 7), 2);
}