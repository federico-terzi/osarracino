//
// Created by max on 16/04/19.
//
#include <gtest/gtest.h>
#include <model/Board.h>
#include <movegenerator/ThetaMoveGenerator.h>
#include <movegenerator/MoveGenerator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <movegenerator/HeisenbergMoveGenerator.h>

template <typename T>
class MoveGeneratorTest : public ::testing::Test {
public:
    using MoveGeneratorType = T;
};

typedef ::testing::Types<ArnoldMoveGenerator, HeisenbergMoveGenerator> MoveGeneratorTypes;
TYPED_TEST_CASE(MoveGeneratorTest, MoveGeneratorTypes);

TYPED_TEST(MoveGeneratorTest, Test_Not_Same_Position) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"WHITE\", \"WHITE\", \"KING\", \"WHITE\", \"WHITE\", \"BLACK\", \"BLACK\"], [\"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test_pos = Position{4,6};
    auto dests = moves::all_from_position(result, test_pos);
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
    EXPECT_EQ(moves::all_from_position(result, test).size(), 10);

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

    EXPECT_EQ(moves::all_from_position(result, test_6).size(), 6);
    EXPECT_EQ(moves::all_from_position(result, test_8).size(), 8);
    EXPECT_EQ(moves::all_from_position(result, test_king).size(), 0);
}

TYPED_TEST(MoveGeneratorTest, Test_Black_out_Returning_in_Citadel) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"BLACK\"], [\"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\", \"BLACK\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"BLACK\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {8, 2};

    EXPECT_EQ(moves::all_from_position(result, test).size(), 0);
}

TYPED_TEST(MoveGeneratorTest, Test_King_can_move) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"WHITE\", \"EMPTY\", \"KING\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position king = Position {2, 2};

    EXPECT_EQ(moves::all_from_position(result, king).size(), 14);
}

TYPED_TEST(MoveGeneratorTest, Test_King_can_move_2) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"WHITE\", \"EMPTY\", \"KING\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position king = Position {2, 2};

    EXPECT_EQ(moves::all_from_position(result, king).size(), 2);
}

TYPED_TEST(MoveGeneratorTest, Test_King_can_move_3) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"WHITE\", \"BLACK\", \"KING\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position king = Position {2, 2};

    EXPECT_EQ(moves::all_from_position(result, king).size(), 0);
}

TYPED_TEST(MoveGeneratorTest, Test_King_can_move_4) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"KING\", \"EMPTY\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position king = Position {2, 4};

    EXPECT_EQ(moves::all_from_position(result, king).size(), 4);
}

TYPED_TEST(MoveGeneratorTest, Test_King_can_move_5) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    Board b;
    std::string board_state {"{\"board\": [[\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"BLACK\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"KING\", \"BLACK\", \"THRONE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"BLACK\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"WHITE\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"], [\"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\", \"EMPTY\"]], \"turn\": \"WHITE\"}"};
    b.load_board(board_state);
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position king = Position {2, 4};

    EXPECT_EQ(moves::all_from_position(result, king).size(), 0);
}

TYPED_TEST(MoveGeneratorTest, Test_black_can_exit_citadel) {
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_black_can_exit_citadel.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {0, 3};

    EXPECT_EQ(moves::all_from_position(result, test).size(), 10);
}

TYPED_TEST(MoveGeneratorTest, Test_central_black_cant_exit_citadel) {
    // It can exit because it is surrounded by other pawns
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_central_black_cant_exit_citadel.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {0, 4};

    EXPECT_EQ(moves::all_from_position(result, test).size(), 0);
}

TYPED_TEST(MoveGeneratorTest, Test_central_black_can_exit_citadel) {
    // Not surrounded anymore
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_central_black_can_exit_citadel.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {0, 4};

    EXPECT_EQ(moves::all_from_position(result, test).size(), 11);
}

TYPED_TEST(MoveGeneratorTest, Test_black_can_move_in_citadel) {
    // Not surrounded anymore
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_black_can_move_in_citadel.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {1, 4};

    EXPECT_EQ(moves::all_from_position(result, test).size(), 11);
}

TYPED_TEST(MoveGeneratorTest, Test_black_cant_enter_back_in_citadel) {
    // Not surrounded anymore
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_black_cant_enter_back_in_citadel.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {0, 7};

    EXPECT_EQ(moves::all_from_position(result, test).size(), 5);
}

TYPED_TEST(MoveGeneratorTest, Test_king_can_move_6) {
    // Not surrounded anymore
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_king_can_move_6.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);
    Position test = Position {2, 6};

    EXPECT_EQ(moves::all_from_position(result, test).size(), 6);
}

TYPED_TEST(MoveGeneratorTest, test_only_one_color_can_move) {
    // Not surrounded anymore
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_only_one_color_can_move.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);

    EXPECT_EQ(result.size(), 23);
}

TYPED_TEST(MoveGeneratorTest, test_only_one_color_can_move_2) {
    // Not surrounded anymore
    using MoveGeneratorType  = typename TestFixture::MoveGeneratorType ;
    auto b {Board::from_path("boards/test_only_one_color_can_move_2.tbbf")};
    MoveGeneratorType generator;
    auto result = generator.generate(b);

    EXPECT_EQ(result.size(), 6);
}