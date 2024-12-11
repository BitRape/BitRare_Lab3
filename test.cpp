#define TESTING
#include <gtest/gtest.h>
#include <mpi.h>
#include "hockeyGame.cpp"

TEST(PassPuckTest, SameTeamPass) {
    std::srand(0);
    HockeyGame game(0, 12);
    game.setCurrentHolder(2);
    game.passPuck();

    EXPECT_NE(game.getCurrentHolder(), 2);
}

TEST(PassPuckTest, OpponentTeamPass) {
    std::srand(0);
    HockeyGame game(0, 12);
    game.setCurrentHolder(2);
    game.passPuck();

    EXPECT_NE(game.getCurrentHolder() / 6, 0);
}

TEST(PassPuckTest, SuccessfulPassesIncrement) {
    std::srand(0);
    HockeyGame game(0, 12);
    game.setCurrentHolder(2); // Player from team 0
    game.passPuck();

    EXPECT_EQ(game.getSuccessfulPasses(), 1);
}

TEST(PassPuckTest, SuccessfulPassesResetOnOpponentPass) {
    std::srand(0);
    HockeyGame game(0, 12);
    game.setCurrentHolder(2); // Player from team 0
    game.setSuccessfulPasses(2);
    game.passPuck();

    EXPECT_EQ(game.getSuccessfulPasses(), 0);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
