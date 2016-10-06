// Created by Nicolo' Valigi
// 2016-10-06
// License: MIT

#pragma once

#include "knightboard.h"

template<typename BOARD>
typename BOARD::PosVec some_path_simple(const BOARD &board,
                                        const typename BOARD::Pos begin,
                                        const typename BOARD::Pos finish,
                                        bool verbose = false) {

    /* **read the README!**
     * We can use DFS to find *a* path from the start to the endpoint. I
     * implemented this just for variety, it isn't any better or easier
     * than BFS for Level 3.
     */

    typename std::unordered_map<typename BOARD::Pos,
            typename BOARD::Pos,
            typename BOARD::PosHasher> parents;

    if (begin == finish) {
        return typename BOARD::PosVec{begin, finish};
    }

    // We're going to use a vector as a stack
    std::vector<typename BOARD::Pos> stack;

    parents.insert({begin, begin});
    stack.push_back(begin);

    while (!stack.empty()) {
        auto this_pos = stack.back();

        if (verbose) {
            std::cout << "Processing " << this_pos << ", finish is " << finish << std::endl;
        }

        if (this_pos == finish) {
            break;
        }

        stack.pop_back();

        for (const auto &adj : board.adjacent_positions(this_pos)) {
            bool was_visited = parents.find(adj.first) != parents.end();
            if (!was_visited) {
                stack.push_back(adj.first);
                parents.insert({adj.first, this_pos});
            }
        }
    }

    // Follow backpointers to find the path
    // We get O(N) in this operation by appending to a vector: N * O(1)
    // then reversing it in one pass: O(N)
    // (could have used a linked list instead)
    typename BOARD::PosVec path;
    auto tmp = parents.at(finish);
    while (tmp != begin) {
        path.push_back(tmp);
        tmp = parents.at(tmp);
    }

    // Push back starting point before reversing
    path.push_back(begin);
    std::reverse(path.begin(), path.end());
    // And push the finish position before returning
    path.push_back(finish);

    return path;
}