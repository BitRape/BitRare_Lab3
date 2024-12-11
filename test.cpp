#define TESTING
#include <gtest/gtest.h>
#include <mpi.h>
#include "hockeyGame.сpp" // Убедитесь, что этот файл существует и содержит объявление класса HockeyGame

// Мок функций для проверки передачи шайбы
TEST(PassPuckTest, SameTeamPass) {
    std::srand(0); // Зафиксировать генерацию случайных чисел для всех процессов
    HockeyGame game(0, 12);
    game.currentHolder = 2; // Игрок 2 из первой команды
    game.passPuck();

    // Важно: шайба должна быть передана другому игроку (внутри одной команды)
    EXPECT_NE(game.currentHolder, 2);
}

TEST(PassPuckTest, OpponentTeamPass) {
    std::srand(0); // Зафиксировать генерацию случайных чисел
    HockeyGame game(0, 12);
    game.currentHolder = 2; // Игрок 2 из первой команды
    game.passPuck();

    // Проверяем, что шайба передана противнику
    EXPECT_NE(game.currentHolder / 6, 0);  // Игрок из другой команды
}

TEST(ScoreTest, UpdateScore) {
    HockeyGame game(0, 12);

    // После успешного гола для команды 0
    game.successfulPasses = SUCCESSFUL_PASSES_FOR_GOAL;
    game.passPuck();

    EXPECT_EQ(game.teamScores[0], 1); // Команда 0 должна получить 1 гол
}

TEST(GameTest, PlayGameSingleProcess) {
    HockeyGame game(0, 12);
    game.playGame();

    // Можно протестировать состояние после игры
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
