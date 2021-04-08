//
// Created by T-bag on 28-Nov-20.
//

#include "TetravexGame.h"
#include <fstream>
#include <string>
TetravexGame::TetravexGame(string name) {
    file_name = name;
    init();
}

TetravexGame::TetravexGame(TetravexGame const &tetravexGame) {
    this->file_name=tetravexGame.file_name;
    init();
}

void TetravexGame::init() {

    int position = 1;
    string line;
    ifstream myfile;
    myfile.open(file_name);
    int line_number = 0;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {

            if (line_number == 0) {
                number_of_lines = (int) line[0] - 48;
                number_of_columns = (int) line[2] - 48;
                unsorted = new piece[number_of_lines*number_of_columns];
                sorted = new piece**[number_of_lines];
                for(int i=0;i<number_of_lines;i++)
                {
                    sorted[i] = new piece*[number_of_columns];
                }
            } else {
                unsorted[line_number - 1].used = false;
                unsorted[line_number - 1].position = position;
                position++;
                unsorted[line_number - 1].numbers[0] = line[0] - 48;
                unsorted[line_number - 1].numbers[1] = line[2] - 48;
                unsorted[line_number - 1].numbers[2] = line[4] - 48;
                unsorted[line_number - 1].numbers[3] = line[6] - 48;
            }
            line_number += 1;
        }
        if (line_number != (number_of_lines * number_of_columns) + 1)
            cout << "error" << endl;
        myfile.close();

    } else cout << "Unable to open file";
}

TetravexGame TetravexGame::getClone() const{
    TetravexGame clone = TetravexGame(*this);
    return  clone;
}