// Created by Nicolo' Valigi
// 2016-10-06
// License: MIT

#pragma once

#include "knightboard.h"

template<typename BOARD>
bool is_valid_step_sequence(const BOARD& board,
                            const std::vector<typename BOARD::Pos> &steps,
                            bool verbose = false) {
    /* STYLE NOTE: I try to reduce the API surface area of classes as much
     * as possible, and implement non-friend external functions. Templates
     * make this quite nice. I think this idea was in one of Meyer's C++ books,
     * and it stuck with me ever since.
     * As long as I can use a const&, I'm generally happy :)
     */

    if (steps.size() < 2) {
        return false;
    }

    bool is_valid = true;

    for (int i = 0; i < steps.size() - 1; i++) {
        is_valid = board.is_valid_step(steps[i], steps[i + 1]);

        if (verbose) {
            board.print(std::cout, steps[i]);

            std::cout << std::endl;
            std::cout << "Step " << i << ": " << steps[i] << " -> " << steps[i + 1]
            << (is_valid ? " [valid] " : " [not valid] ") << std::endl << std::endl;

            // Print the board one last time for the final position
            if (i == steps.size() - 2) {
                board.print(std::cout, steps[i + 1]);
            }
        }

        if (!is_valid) {
            // We could break here if we wanted to skip the rest of a non-valid
            // sequence
            // break;
        }
    }

    return is_valid;
}