//
// Created by T-bag on 28-Nov-20.
//

#ifndef TETRAVEXGAME_TETRAVEXSOLVER_H
#define TETRAVEXGAME_TETRAVEXSOLVER_H
#include "ThreadPool.h"
#include "../game/TetravexGame.h"
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3


class TetravexSolver {
    int solutionIndex=0;
    vector<TetravexGame> listGame;
    mutex m;
    atomic<int> compteur;
    bool solved = false;
    TetravexGame tetravexGame;
public:

    TetravexSolver(TetravexGame game);
    void print_solved(int index);
    void print_solvedPool(int index,chrono::time_point<chrono::high_resolution_clock> time_start);
    void launchThread(int i);
    void launchThreadPool(int i,chrono::time_point<chrono::high_resolution_clock> time_start);
    bool find_next_piece_sequential(int x_pos, int y_pos);
    bool find_next_piece_threadPool(int x_pos, int y_pos, int start, int end, int gameIndex,chrono::time_point<chrono::high_resolution_clock> time_start);
    bool find_next_piece_multi_thread(int x_pos, int y_pos, int start, int end, int gameIndex);
    void solveSequentialBacktracking(int x_pos, int y_pos);
    void solveMultiThreadBacktracking(int x_pos, int y_pos);
    void solveThreadPool(int x_pos, int y_pos, size_t size);
};


#endif //TETRAVEXGAME_TETRAVEXSOLVER_H
