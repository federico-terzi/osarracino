//
// Created by max on 16/04/19.
//
#include <gtest/gtest.h>
#include <model/Board.h>
#include <movegenerator/ThetaMoveGenerator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>

template <typename T>
class MoveGeneratorTest : public ::testing::Test {
public:
    using MoveGeneratorType = T;
};

typedef ::testing::Types<ArnoldMoveGenerator, ThetaMoveGenerator> MoveGeneratorTypes;
TYPED_TEST_CASE(MoveGeneratorTest, MoveGeneratorTypes);

TYPED_TEST(MoveGeneratorTest, Test_Not_Same_Position) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test_pos = Position{4,6};
    auto dests = result[test_pos];
    for(const Position &pos : dests) {
        EXPECT_FALSE(pos == test_pos) << pos;
    }
}

TYPED_TEST(MoveGeneratorTest, Test_Board_Edge) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK\""};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = {0, 2};
    EXPECT_EQ(result[test].size(), 10);

}

TYPED_TEST(MoveGeneratorTest, Test_Generation_Start_For_Whites) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state  {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test_8 = Position{4,6};
    Position test_6 = Position{5,4};
    Position test_king = Position{4, 4};

    EXPECT_EQ(result[test_6].size(), 6);
    EXPECT_EQ(result[test_8].size(), 8);
    EXPECT_EQ(result[test_king].size(), 0);
}

TYPED_TEST(MoveGeneratorTest, Test_Black_out_Returning_in_Citadel) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {8, 2};

    EXPECT_EQ(result[test].size(), 0);
}