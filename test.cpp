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

TEST(ScoreTest, UpdateScore) {
    HockeyGame game(0, 12);

    game.setSuccessfulPasses(SUCCESSFUL_PASSES_FOR_GOAL);
    game.passPuck();
    EXPECT_EQ(game.getTeamScore(0), 1);
}

TEST(GameTest, PlayGameSingleProcess) {
    HockeyGame game(0, 12);
    game.playGame();

    EXPECT_GE(game.getTeamScore(0), 0);
    EXPECT_GE(game.getTeamScore(1), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
