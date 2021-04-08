//
// Created by T-bag on 28-Nov-20.
//

#ifndef TETRAVEXGAME_TETRAVEXGAME_H
#define TETRAVEXGAME_TETRAVEXGAME_H

#include <iostream>
struct piece {
    int position;
    bool used;
    int numbers[4];
};

using namespace std;
class TetravexGame {
public:
    string file_name;
    int number_of_lines;
    int number_of_columns;
    piece *unsorted;
    piece ***sorted;
    TetravexGame(TetravexGame const &tetravexGame);
    TetravexGame(string name);
    void init();
    TetravexGame getClone() const;
};


#endif //TETRAVEXGAME_TETRAVEXGAME_H
