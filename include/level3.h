// Created by Nicolo' Valigi
// 2016-10-06
// License: MIT

#pragma once

#include "knightboard.h"

template<typename BOARD>
typename BOARD::PosVec shortest_path_simple(const BOARD &board,
                                            const typename BOARD::Pos begin,
                                            const typename BOARD::Pos finish,
                                            const bool verbose = false) {

    /* **read the README!**
     * This is just BFS. I use a map instead of a vector to keep visited status
     * and parent backlinks to save memory in the common case, and optimize
     * away the O(N) initialization at the beginning.
     *
     * @returns a list of **intermediate** steps (i.e. start and finish are
     * not included)
     */

    typename std::unordered_map<typename BOARD::Pos,
            typename BOARD::Pos,
            typename BOARD::PosHasher> parents;

    if (begin == finish) {
        return typename BOARD::PosVec{begin, finish};
    }

    std::queue<typename BOARD::Pos> queue;

    // Enqueue starting point
    parents.insert({begin, begin});
    // parents[begin] = begin;
    queue.push(begin);

    while (!queue.empty()) {
        auto this_pos = queue.front();

        if (verbose) {
            std::cout << "Processing " << this_pos << std::endl;
        }

        if (this_pos == finish) {
            break;
        }

        queue.pop();

        for (const auto &adj : board.adjacent_positions(this_pos)) {
            bool was_visited = parents.find(adj.first) != parents.end();
            if (!was_visited) {
                // set parent pointer
                parents.insert({adj.first, this_pos});
                queue.push(adj.first);
            }
        }
    }

    // Recover the path, same code as the Level 2 solution
    typename BOARD::PosVec path;
    auto tmp = parents.at(finish);
    while (tmp != begin) {
        path.push_back(tmp);
        tmp = parents.at(tmp);
    }
    path.push_back(begin);
    std::reverse(path.begin(), path.end());
    path.push_back(finish);

    return path;
}