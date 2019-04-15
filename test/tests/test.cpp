#include <model/Board.h>
#include <unordered_map>
#include <functional>
#include <evaluator/Evaluator.h>
#include "gtest/gtest.h"

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}