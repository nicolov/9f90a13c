// Created by Nicolo' Valigi
// 2016-10-06
// License: MIT

#pragma once

#include "knightboard.h"

#include <memory>
#include <bitset>

template<typename BOARD>
struct BoardDPTypes {
    /*
     * The dynamic programming table for this is n * (2^n), where
     * the first index is the ending position of the path, and the second
     * a bitset that flags visited vertices for that specific path.
     */
    static constexpr size_t num_squares = BOARD::size * BOARD::size;
    static constexpr size_t visited_bitfield_combinations = (1 << num_squares);

    using Table = std::array<std::array<int, visited_bitfield_combinations>, num_squares>;

    // Bitset used to flag already visited vertices
    using VisitedMask = std::bitset<num_squares>;

    static const int NOT_VISITED = -2;
};


/*
 * Finds the longest path ending at pos p only visiting vertices
 * allowed by the bitset mask.
 * Returns -1 if there are no paths that satisfy the end_pos and
 * visited_mask constraints.
 */
template <typename BOARD>
int longest_path_solve(const BOARD &board,
                       const typename BOARD::Pos &end_pos,
                       const typename BoardDPTypes<BOARD>::VisitedMask visited_mask,
                       typename BoardDPTypes<BOARD>::Table &table) {
    using BDPT = BoardDPTypes<BOARD>;

    // return 0 if this the bit corresponding to this end_pos
    // is the only 1 in visited_mask (meaning that there are no
    // other possible vertices to visit).
    typename BDPT::VisitedMask only_me_mask;
    only_me_mask.reset();
    only_me_mask.set(end_pos.as_int());

    if (only_me_mask == visited_mask) {
        return 0;
    }

    auto &this_result_in_table = table[end_pos.as_int()][visited_mask.to_ulong()];

    bool to_visit = (this_result_in_table == BDPT::NOT_VISITED);
    if (to_visit) {
        int longest_ending_here = -1;
        // Loop through adjacent positions to this square
        for (const auto &adj : board.adjacent_positions(end_pos)) {
            // Only consider squares that visited_mask allows us to use
            // for this subproblem
            if (visited_mask.test(adj.first.as_int())) {
                // Solve the new subproblem that we obtain by ending at
                // adj.first **without** using the current ending position
                auto new_mask = visited_mask;
                new_mask.reset(end_pos.as_int());
                auto longest_through_this = longest_path_solve(board,
                                                               adj.first,
                                                               new_mask,
                                                               table);

                if (longest_through_this != 1) {
                    // Add 1 for the path between adj.first and end_pos
                    longest_through_this += 1;
                    if (longest_through_this > longest_ending_here) {
                        // Found a longer path
                        longest_ending_here = longest_through_this;
                    }
                }
            }
        }
        // Set the longest path obtained by looking at all possible
        // moves from here
        this_result_in_table = longest_ending_here;
    }

    return this_result_in_table;
}

template <typename BOARD>
int longest_path_dp(const BOARD &board) {
    // Good luck stack-allocating this :)
    std::unique_ptr<typename BoardDPTypes<BOARD>::Table> table(new typename BoardDPTypes<BOARD>::Table());

    // Initialize the table
    for (auto &r : *table) {
        for (auto &elem : r) {
            elem = BoardDPTypes<BOARD>::NOT_VISITED;
        }
    }

    int longest_any_end = -1;
    for (int end_index = 0; end_index < BoardDPTypes<BOARD>::num_squares; end_index++) {
        typename BOARD::Pos end_pos(end_index);
        // The mask for this element will be all ones save for this endpoint itself
        typename BoardDPTypes<BOARD>::VisitedMask mask;
        mask.set();
        mask.reset(end_index);
        int longest_ending_here = longest_path_solve(board, end_pos, mask, *table);

        if (longest_ending_here > longest_any_end) {
            longest_any_end = longest_ending_here;
        }
    }

    return longest_any_end;
}