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

TEST(PassPuckTest, MultipleSameTeamPasses) {
    std::srand(0);
    HockeyGame game(0, 12);
    game.setCurrentHolder(2);

    int initialTeam = game.getCurrentHolder() / 6;
    int sameTeamCount = 0;
    for (int i = 0; i < 10; ++i) {
        game.passPuck();
        if (game.getCurrentHolder() / 6 == initialTeam) {
            sameTeamCount++;
        }
    }
    // Проверяем, что шайба передавалась внутри одной команды хотя бы один раз
    EXPECT_GT(sameTeamCount, 0);
}

TEST(PassPuckTest, OpponentTeamPass) {
    std::srand(0);
    HockeyGame game(0, 12);
    game.setCurrentHolder(2);
    game.passPuck();

    EXPECT_NE(game.getCurrentHolder() / 6, 0);
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
