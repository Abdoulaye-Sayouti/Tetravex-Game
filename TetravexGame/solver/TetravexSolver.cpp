//
// Created by T-bag on 28-Nov-20.
//

#include "TetravexSolver.h"

TetravexSolver::TetravexSolver(TetravexGame game) : tetravexGame(game) {};



void TetravexSolver::print_solvedPool(int index, chrono::time_point<std::chrono::high_resolution_clock> time_start) {



	auto end = chrono::high_resolution_clock::now();
        for (int x = 0; x < listGame[index].number_of_lines; x++) {
            for (int y = 0; y < listGame[index].number_of_columns; y++) {
                cout << x + 1 << " " << y + 1 << ": " << listGame[index].sorted[x][y]->position << endl;
            }
        }

    
    
    
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - time_start).count();
    
    int seconds = duration / 1000;
    int milisec = duration - seconds * 1000;
    int minutes = seconds / 60;
    seconds = seconds - minutes * 60;
    cout << "Solving with ThreadPool :" << endl << "Time = " << minutes << "m" << seconds << "."
             << milisec << endl;

}


void TetravexSolver::print_solved(int index) {
 
    if (index == -1 ) {
        for (int x = 0; x < tetravexGame.number_of_lines; x++) {
            for (int y = 0; y < tetravexGame.number_of_columns; y++) {
                cout << x + 1 << " " << y + 1 << ": " << tetravexGame.sorted[x][y]->position << endl;
            }
        }

    } else {
        for (int x = 0; x < listGame[index].number_of_lines; x++) {
            for (int y = 0; y < listGame[index].number_of_columns; y++) {
                cout << x + 1 << " " << y + 1 << ": " << listGame[index].sorted[x][y]->position << endl;
            }
        }
    }


}

////////////////////////////   Sequential Backtracking   ///////////////////////
bool TetravexSolver::find_next_piece_sequential(int x_pos, int y_pos) {
    if (solved)
        return true;

    if (x_pos == tetravexGame.number_of_lines) {
        solved = true;
        return (true);
    } else {
        for (int i = 0; i < tetravexGame.number_of_lines * tetravexGame.number_of_columns; i++) {
            if (tetravexGame.unsorted[i].used == false) {
                if (((x_pos == 0) ||
                     (tetravexGame.sorted[x_pos - 1][y_pos]->numbers[DOWN] == tetravexGame.unsorted[i].numbers[UP]))
                    && ((y_pos == 0) || (tetravexGame.sorted[x_pos][y_pos - 1]->numbers[RIGHT] ==
                                         tetravexGame.unsorted[i].numbers[LEFT]))) {
                    tetravexGame.sorted[x_pos][y_pos] = &tetravexGame.unsorted[i];
                    tetravexGame.unsorted[i].used = true;
                    int new_x_pos = x_pos;
                    int new_y_pos = (y_pos + 1);
                    if (new_y_pos == tetravexGame.number_of_columns) {
                        new_y_pos = 0;
                        new_x_pos++;
                    }
                    if (find_next_piece_sequential(new_x_pos, new_y_pos)) {
                        return (true);
                    }
                    tetravexGame.unsorted[i].used = false;
                }
            }
        }
    }
    return (false);
}

void TetravexSolver::solveSequentialBacktracking(int x_pos, int y_pos) {
    auto start = chrono::high_resolution_clock::now();
    find_next_piece_sequential(x_pos, y_pos);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    int seconds = duration / 1000;
    int milisec = duration - seconds * 1000;
    int minutes = seconds / 60;
    seconds = seconds - minutes * 60;
    if (solved) {
        print_solved(-1);
        cout << "Solving with Sequential Backtracking :"<< endl << "Time = " << minutes << "m" << seconds << "."
             << milisec << endl;

    } else
        cout << "No solution found" << endl;

    delete[]  tetravexGame.unsorted;
    for (int i = 0; i < tetravexGame.number_of_lines; i++)
        delete[] tetravexGame.sorted[i];
    delete[] tetravexGame.sorted;
}

////////////////////////////////  Multithread ////////////////////////////////////////
bool TetravexSolver::find_next_piece_multi_thread(int x_pos, int y_pos, int start, int end, int gameIndex) {


    if (compteur >0)
        return true;

    if (x_pos == listGame[gameIndex].number_of_lines) {
    
        solutionIndex = gameIndex;
        compteur++;
        solved =true;
        return (true);
    } else {
        for (int i = 0; i < listGame[gameIndex].number_of_lines * listGame[gameIndex].number_of_columns; i++) {
            if (listGame[gameIndex].unsorted[i].used == false) {
                if (x_pos == 0 && y_pos == 0 && i >= start && i < end) {
                    listGame[gameIndex].sorted[x_pos][y_pos] = &listGame[gameIndex].unsorted[i];
                    listGame[gameIndex].unsorted[i].used = true;
                    int new_x_pos = x_pos;
                    int new_y_pos = (y_pos + 1);
                    if (find_next_piece_multi_thread(new_x_pos, new_y_pos, start, end, gameIndex)) {
                        return (true);
                    }
                    listGame[gameIndex].unsorted[i].used = false;
                }
                else if(x_pos != 0 || y_pos != 0 ){

                    if (((x_pos == 0) || (listGame[gameIndex].sorted[x_pos - 1][y_pos]->numbers[DOWN] ==
                                          listGame[gameIndex].unsorted[i].numbers[UP]))
                        && ((y_pos == 0) || (listGame[gameIndex].sorted[x_pos][y_pos - 1]->numbers[RIGHT] ==
                                             listGame[gameIndex].unsorted[i].numbers[LEFT]))) {
                        listGame[gameIndex].sorted[x_pos][y_pos] = &listGame[gameIndex].unsorted[i];
                        listGame[gameIndex].unsorted[i].used = true;
                        int new_x_pos = x_pos;
                        int new_y_pos = (y_pos + 1);
                        if (new_y_pos == listGame[gameIndex].number_of_columns) {
                            new_y_pos = 0;
                            new_x_pos++;
                        }
                        if (find_next_piece_multi_thread(new_x_pos, new_y_pos, start, end, gameIndex)) {
                            return (true);
                        }
                        listGame[gameIndex].unsorted[i].used = false;
                    }
                }

            }
        }
    }
    return (false);
}

void TetravexSolver::launchThread(int i) {
    find_next_piece_multi_thread(0, 0, i * tetravexGame.number_of_lines,
                                 (i + 1) * tetravexGame.number_of_lines, i);
}

void TetravexSolver::solveMultiThreadBacktracking(int x_pos, int y_pos) {


    for (int i = 0; i < tetravexGame.number_of_columns; i++) {
        listGame.push_back(tetravexGame.getClone());
    }
    /*
    thread t(&TetravexSolver::find_next_piece_multi_thread,this,0, 0, 0,36, 0);
    auto start = chrono::high_resolution_clock::now();
    t.join();
    /*/

    compteur.store(0);
    thread *t = new thread[tetravexGame.number_of_lines];


    for (int i = 0; i < tetravexGame.number_of_columns; i++) {

        t[i] = thread(&TetravexSolver::launchThread, this,i);

    }

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < tetravexGame.number_of_columns; i++) {
        if(t[i].joinable())
            t[i].join();
    }//*/

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    delete [] t;



    int seconds = duration / 1000;
    int milisec = duration - seconds * 1000;
    int minutes = seconds / 60;
    seconds = seconds - minutes * 60;

    if (solved) {
        print_solved(solutionIndex);
        cout << "Solving with Multi Thread Backtracking :" << endl << "Time = " << minutes << "m" << seconds << "."
             << milisec << endl;

    } else
        cout << "No solution found" << endl;

    delete[]  tetravexGame.unsorted;
    for (int i = 0; i < tetravexGame.number_of_lines; i++)
        delete[] tetravexGame.sorted[i];
    delete[] tetravexGame.sorted;

}

///////////////////////////// TheadPool  ///////////////////////////
bool TetravexSolver::find_next_piece_threadPool(int x_pos, int y_pos, int start, int end, int gameIndex, std::chrono::time_point<std::chrono::high_resolution_clock> time_start) {


    if (compteur >0)
        return true;

    if (x_pos == listGame[gameIndex].number_of_lines) {
    	
        solutionIndex = gameIndex;
        //print_solved(solutionIndex);
        print_solvedPool(solutionIndex,time_start);
        compteur++;
        solved =true;
        exit(0);
        return (true);
    } else {
        for (int i = 0; i < listGame[gameIndex].number_of_lines * listGame[gameIndex].number_of_columns; i++) {
            if (listGame[gameIndex].unsorted[i].used == false) {
                if (x_pos == 0 && y_pos == 0 && i >= start && i < end) {
                    listGame[gameIndex].sorted[x_pos][y_pos] = &listGame[gameIndex].unsorted[i];
                    listGame[gameIndex].unsorted[i].used = true;
                    int new_x_pos = x_pos;
                    int new_y_pos = (y_pos + 1);
                    if (find_next_piece_threadPool(new_x_pos, new_y_pos, start, end, gameIndex,time_start)) {
                        return (true);
                    }
                    listGame[gameIndex].unsorted[i].used = false;
                }
                else if(x_pos != 0 || y_pos != 0 ){

                    if (((x_pos == 0) || (listGame[gameIndex].sorted[x_pos - 1][y_pos]->numbers[DOWN] ==
                                          listGame[gameIndex].unsorted[i].numbers[UP]))
                        && ((y_pos == 0) || (listGame[gameIndex].sorted[x_pos][y_pos - 1]->numbers[RIGHT] ==
                                             listGame[gameIndex].unsorted[i].numbers[LEFT]))) {
                        listGame[gameIndex].sorted[x_pos][y_pos] = &listGame[gameIndex].unsorted[i];
                        listGame[gameIndex].unsorted[i].used = true;
                        int new_x_pos = x_pos;
                        int new_y_pos = (y_pos + 1);
                        if (new_y_pos == listGame[gameIndex].number_of_columns) {
                            new_y_pos = 0;
                            new_x_pos++;
                        }
                        if (find_next_piece_threadPool(new_x_pos, new_y_pos, start, end, gameIndex,time_start)) {
                            return (true);
                        }
                        listGame[gameIndex].unsorted[i].used = false;
                    }
                }

            }
        }
    }
    return (false);
}

void TetravexSolver::launchThreadPool(int i, std::chrono::time_point<std::chrono::high_resolution_clock> time_start) {
    find_next_piece_threadPool(0, 0, i * tetravexGame.number_of_lines,
                                 (i + 1) * tetravexGame.number_of_lines, i,time_start);
}




void TetravexSolver::solveThreadPool(int x_pos, int y_pos, size_t nbThreads){
ThreadPool pool{nbThreads};
    
    
    for (int i = 0; i < tetravexGame.number_of_columns; i++) {
        listGame.push_back(tetravexGame.getClone());
    }

    compteur.store(0);
    

    std::chrono::time_point<std::chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();

	for (int i = 0; i < tetravexGame.number_of_columns; i++) {

         pool.addTask([this,i,start] {
	  launchThreadPool(i,start);
    });
    

    }
    


}

