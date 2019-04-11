#include <Board.h>
#include "gtest/gtest.h"

TEST(Test_isTurn, test_turn_white) {
    Board b;
    b.load_board("{\"board\":[[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"BLACK\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\"],[\"BLACK\",\"BLACK\",\"WHITE\",\"WHITE\",\"KING\",\"WHITE\",\"WHITE\",\"BLACK\",\"BLACK\"],[\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"BLACK\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\"]],\"turn\":\"WHITE\"}");
    EXPECT_TRUE(b.is_white) << "Turn is not white";
}

TEST(Test_isTurn, test_turn_black) {
    Board b;
    b.load_board("{\"board\":[[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"BLACK\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\"],[\"BLACK\",\"BLACK\",\"WHITE\",\"WHITE\",\"KING\",\"WHITE\",\"WHITE\",\"BLACK\",\"BLACK\"],[\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"WHITE\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\",\"EMPTY\"],[\"EMPTY\",\"EMPTY\",\"EMPTY\",\"BLACK\",\"BLACK\",\"BLACK\",\"EMPTY\",\"EMPTY\",\"EMPTY\"]],\"turn\":\"BLACK\"}");
    EXPECT_FALSE(b.is_white) << "Turn is not black";
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}