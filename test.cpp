#include <gtest/gtest.h>
using "hockeyGame.cpp";

TEST(PassPuckTest, SameTeamPass) {
    std::srand(0); 
    HockeyGame game(0, 12);
    game.currentHolder = 2;  
    game.passPuck();

    EXPECT_NE(game.currentHolder, 2); 
}

TEST(PassPuckTest, OpponentTeamPass) {
    std::srand(0);  
    HockeyGame game(0, 12);
    game.currentHolder = 2; 
    game.passPuck();

    EXPECT_NE(game.currentHolder / 6, 0); 
}

TEST(ScoreTest, UpdateScore) {
    HockeyGame game(0, 12);

    game.successfulPasses = SUCCESSFUL_PASSES_FOR_GOAL;
    game.passPuck();
    EXPECT_EQ(game.teamScores[0], 1);
}

TEST(GameTest, PlayGameSingleProcess) {
    HockeyGame game(0, 12);
    game.playGame();
    
    EXPECT_GE(game.teamScores[0], 0);
    EXPECT_GE(game.teamScores[1], 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}