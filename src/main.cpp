// Created by Nicolo' Valigi
// 2016-10-06
// License: MIT

#include "knightboard.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level5.h"

#include <iostream>

using namespace std;

int main() {
    /*
     * See test/test.cpp for the real stuff,
     * these are just examples..
     */

    Board8 board;

    //
    // Level 1
    board.print(cout);
    cout << endl << endl;

    board.print(cout, Pos8(4,4));
    cout << endl << endl;

    is_valid_step_sequence(board, {Pos8(0, 0), Pos8(2, 1), Pos8(3, 3)}, true);
    shortest_path_simple(board, {0, 0}, {2, 1});

    //
    // Level 4
    Board32 board32;
    board32.load_from_file();

    is_valid_step_sequence(board32, shortest_path_lvl4(board32, {9, 30}, {26, 0}), true);

    // I haven't really tested the solution for Level 5
    // Board<5> board5;
    // std::cout << longest_path_dp(b) << std::endl;
}