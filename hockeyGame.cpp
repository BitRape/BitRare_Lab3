#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int TEAM_SIZE = 6;
const int NUM_TEAMS = 2;
const int PASS_LIMIT = 2000;
const int SUCCESSFUL_PASSES_FOR_GOAL = 4;
const double PROBABILITY_SAME_TEAM = 0.65;
const double PROBABILITY_OTHER_TEAM = 0.35;

class HockeyGame {
public:
    HockeyGame(int rank, int size);
    void playGame();

    // Публичные методы для доступа к приватным членам
    int getCurrentHolder() const { return currentHolder; }
    void setCurrentHolder(int holder) { currentHolder = holder; }
    int getSuccessfulPasses() const { return successfulPasses; }
    void setSuccessfulPasses(int passes) { successfulPasses = passes; }
    int getTeamScore(int team) const { return teamScores[team]; }
    void passPuck();

private:
    int rank;
    int size;
    int teamScores[NUM_TEAMS];
    int currentHolder;
    int successfulPasses;

    void getScore();
};

HockeyGame::HockeyGame(int rank, int size) : rank(rank), size(size), successfulPasses(0), currentHolder(0) {
    for (int i = 0; i < NUM_TEAMS; ++i) {
        teamScores[i] = 0;
    }
}

void HockeyGame::passPuck() {
    int team = currentHolder / TEAM_SIZE;

    double randomValue = (double)std::rand() / RAND_MAX;
    int nextHolder;

    if (randomValue < PROBABILITY_SAME_TEAM) {
        do {
            nextHolder = team * TEAM_SIZE + std::rand() % TEAM_SIZE;
        } while (nextHolder == currentHolder);
        successfulPasses++;
    } else {
        nextHolder = (1 - team) * TEAM_SIZE + std::rand() % TEAM_SIZE;
        successfulPasses = 0;
    }

    // Проверка на гол
    if (successfulPasses == SUCCESSFUL_PASSES_FOR_GOAL) {
        teamScores[team]++;
        successfulPasses = 0;

        nextHolder = (1 - team) * TEAM_SIZE + std::rand() % TEAM_SIZE;
    }

    currentHolder = nextHolder;
}

void HockeyGame::getScore() {
    std::cout << "Final Score:\n";
    std::cout << "Team 1: " << teamScores[0] << "\n";
    std::cout << "Team 2: " << teamScores[1] << "\n";
}

void HockeyGame::playGame() {
    int passes = 0;

    while (passes < PASS_LIMIT) {
        if (rank == 0) {
            passPuck();
        }

        MPI_Bcast(&currentHolder, 1, MPI_INT, 0, MPI_COMM_WORLD);

        passes++;
    }

    int globalScores[NUM_TEAMS] = {0};
    MPI_Reduce(teamScores, globalScores, NUM_TEAMS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        getScore();
    }
}

#ifndef TESTING
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != TEAM_SIZE * NUM_TEAMS) {
        if (rank == 0) {
            std::cerr << "The program requires exactly " << TEAM_SIZE * NUM_TEAMS << " processes.\n";
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    std::srand(static_cast<unsigned>(std::time(0)) + rank * 100 + static_cast<int>(MPI_Wtime() * 1000));// Инициализация генератора случайных чисел

    HockeyGame game(rank, size);
    game.playGame();

    MPI_Finalize();
    return 0;
}
#endif
