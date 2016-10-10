// Created by Nicolo' Valigi
// 2016-10-06
// License: MIT

#include <gtest/gtest.h>
#include <iostream>

#include "knightboard.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"

class Board8Test : public ::testing::Test {
protected:
    virtual void SetUp() {}

    Board8 board;
};


TEST_F(Board8Test, out_of_bounds_steps) {
    EXPECT_EQ(false, board.is_valid_step(Pos8(7, 7), Pos8(8, 9)));
    EXPECT_EQ(false, board.is_valid_step(Pos8(1, 1), Pos8(-1, 0)));
}

TEST_F(Board8Test, individual_steps) {
    EXPECT_EQ(true, board.is_valid_step(Pos8(1, 1), Pos8(3, 2)));
    EXPECT_EQ(true, board.is_valid_step(Pos8(6, 6), Pos8(5, 4)));

    EXPECT_EQ(false, board.is_valid_step(Pos8(4, 4), Pos8(5, 5)));
    EXPECT_EQ(false, board.is_valid_step(Pos8(4, 4), Pos8(3, 3)));
}

TEST_F(Board8Test, sequence) {
    EXPECT_EQ(false, is_valid_step_sequence(board, PosVec8()));
    EXPECT_EQ(false, is_valid_step_sequence(board, {Pos8(0, 0), Pos8(1, 1)}));
    EXPECT_EQ(true, is_valid_step_sequence(board, {Pos8(0, 0), Pos8(2, 1)}));
    EXPECT_EQ(true, is_valid_step_sequence(board, {Pos8(7, 7), Pos8(6, 5), Pos8(4, 4)}));
    EXPECT_EQ(false, is_valid_step_sequence(board, {Pos8(0, 0), Pos8(-2, -1)}));
}

TEST_F(Board8Test, adjacent_positions) {
    EXPECT_EQ(8, board.adjacent_positions({4, 4}).size());
    GraphEdgeVec8 v1{{Pos8(1, 2), 1},
                     {Pos8(2, 1), 1}};
    EXPECT_EQ(v1, board.adjacent_positions({0, 0}));
}

TEST_F(Board8Test, some_path_simple) {
    auto v0 = some_path_simple(board, {0, 0}, {2, 1});
    EXPECT_EQ(true, is_valid_step_sequence(board, v0));

    auto v1 = some_path_simple(board, {0, 0}, {4, 0});
    EXPECT_EQ(true, is_valid_step_sequence(board, v1));

    auto v2 = some_path_simple(board, {0, 0}, {6, 1});
    EXPECT_EQ(true, is_valid_step_sequence(board, v2));
}

TEST_F(Board8Test, shortest_path_simple) {
    EXPECT_EQ(2, shortest_path_simple(board, {0, 0}, {2, 1}).size());
    PosVec8 v1{{0, 0},
               {2, 1},
               {4, 0}};
    EXPECT_EQ(v1, shortest_path_simple(board, {0, 0}, {4, 0}));
    PosVec8 v2{{0, 0},
               {2, 1},
               {4, 0},
               {6, 1}};
    EXPECT_EQ(v2, shortest_path_simple(board, {0, 0}, {6, 1}));
}

class Board32Test : public ::testing::Test {
protected:
    virtual void SetUp() {
        board.load_from_file();
    }

    Board32 board;
};

TEST_F(Board32Test, check_barriers) {
    EXPECT_EQ(false, board.is_valid_step({0, 7}, {1, 8})); // Cross
    EXPECT_EQ(false, board.is_valid_step({0, 8}, {1, 10})); // Start
    EXPECT_EQ(false, board.is_valid_step({1, 10}, {0, 8})); // Finish
}

TEST_F(Board32Test, check_rocks) { // rock is on i: 9-10, j: 3-4 
    EXPECT_EQ(false, board.is_valid_step({7, 2}, {9, 3})); // Finish on rock
    EXPECT_EQ(true, board.is_valid_step({9, 2}, {11, 3})); // Cross rock
}

TEST_F(Board32Test, shortest_path_simple_w_map) {
    EXPECT_EQ(2, shortest_path_simple(board, {0, 0}, {2, 1}).size());
}

TEST_F(Board32Test, shortest_path_lvl4) {
    PosVec32 v0{{0, 0},
                {2, 1}};
    EXPECT_EQ(v0, shortest_path_lvl4(board, {0, 0}, {2, 1}));

    PosVec32 v1{{0, 0},
                {2, 1},
                {4, 0}};
    EXPECT_EQ(v1, shortest_path_lvl4(board, {0, 0}, {4, 0}));
    EXPECT_EQ(true, is_valid_step_sequence(board, v1));

    PosVec32 v2{{0, 0},
                {2, 1},
                {4, 2},
                {6, 1}};
    EXPECT_EQ(v2, shortest_path_lvl4(board, {0, 0}, {6, 1}));
    EXPECT_EQ(true, is_valid_step_sequence(board, v2));

    PosVec32 v3{{16, 0},
                {18, 1},
                {20, 2}};
    EXPECT_EQ(v3, shortest_path_lvl4(board, {16, 0}, {20, 2}));
    EXPECT_EQ(true, is_valid_step_sequence(board, v3));

    // A nice tour of the map..
    EXPECT_EQ(29, shortest_path_lvl4(board, {0, 0}, {0, 10}).size());
}

TEST_F(Board32Test, teleportation) {
    // Trivial teleport case
    PosVec32 v0{{11, 26},
                {23, 27}};
    EXPECT_EQ(v0, shortest_path_lvl4(board, {11, 26}, {23, 27}));
    EXPECT_EQ(true, is_valid_step_sequence(board, v0));

    // Start from teleport and finish one step from the destination
    PosVec32 v1{{11, 26},
                {25, 28}};
    EXPECT_EQ(v1, shortest_path_lvl4(board, {11, 26}, {25, 28}));
    EXPECT_EQ(true, is_valid_step_sequence(board, v0));

    // Take good advantage of the teleport
    auto v3 = shortest_path_lvl4(board, {9, 30}, {26, 30});
    EXPECT_EQ(true, is_valid_step_sequence(board, v3));
}

TEST_F(Board32Test, looong_path) {
    auto v0 = shortest_path_lvl4(board, {9, 30}, {26, 0});
    EXPECT_EQ(true, is_valid_step_sequence(board, v0));
}