#include "gtest/gtest.h"

TEST(test, test_eq) {
    EXPECT_EQ(1, 0);
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}