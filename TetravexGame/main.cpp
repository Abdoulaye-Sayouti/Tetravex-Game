#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <inttypes.h>

#include "solver/TetravexSolver.h"
#include "solver/ThreadPool.h"

using namespace std;

int main(void) {
    size_t nbThreads = 4;
    TetravexGame tetravexGame("data/5x5.txt");
    TetravexSolver tetravexSolver(tetravexGame);

    ///////////// SequantialBacktracking   ///////////////////
    //tetravexSolver.solveSequentialBacktracking(0,0);

    ////////////////////////  MultiThreadBacktracking ///////////////////
    tetravexSolver.solveMultiThreadBacktracking(0,0);

    ////////////////////////// ThreadPool  /////////////////////////////
    
    //tetravexSolver.solveThreadPool(0,0,nbThreads);

    return (0);
}


