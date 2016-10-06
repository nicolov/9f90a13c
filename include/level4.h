// Created by Nicolo' Valigi
// 2016-10-06
// License: MIT

#pragma once

#include "knightboard.h"

template <typename BOARD>
typename BOARD::PosVec shortest_path_lvl4(const BOARD &board,
                                          const typename BOARD::Pos begin,
                                          const typename BOARD::Pos finish,
                                          const bool verbose = false) {

    /* **read the README!**
     * This is just Dijkstra.
     *
     * @returns a list of **intermediate** steps (i.e. start and finish are
     * not included)
     */

    if (begin == finish) {
        return typename BOARD::PosVec{begin, finish};
    }

    // Trivial teleport case
    if ((board.b[begin.x][begin.y] == BoardSquare::Teleport) && (board.b[finish.x][finish.y] == BoardSquare::Teleport)) {
        return typename BOARD::PosVec{begin, finish};
    }

    // Usual book-keeping for Dijkstra's
    struct DijkstraData {
        typename BOARD::Pos parent;
        int dist;
    };

    using DijkstraMap = std::unordered_map<typename BOARD::Pos, DijkstraData, typename BOARD::PosHasher>;

    DijkstraMap explored;

    // Defines ordering for the GraphQueue priority queue
    // Could have used a lambda here, but it ends up being more verbose with decltype
    struct EdgeWeightPriorityFunctor {
        bool operator()(const typename BOARD::GraphEdge &e1, const typename BOARD::GraphEdge &e2) {
            return e1.second > e2.second;
        }
    };

    using DijkstraQueue = std::priority_queue<typename BOARD::GraphEdge,
            typename BOARD::GraphEdgeVec,
            EdgeWeightPriorityFunctor>;

    DijkstraQueue queue;

    // The starting point has distance 0 to itself
    explored.insert({begin, {begin, 0}});
    // ...and.. we start
    queue.push({begin, 0});

    while (!queue.empty()) {
        auto this_edge = queue.top();
        auto this_pos = this_edge.first;

        if (verbose) {
            std::cout << "Processing " << this_pos << std::endl;
        }

        if (this_pos == finish) {
            break;
        }

        queue.pop();

        for (const auto &adj : board.adjacent_positions(this_pos)) {
            bool was_visited = explored.find(adj.first) != explored.end();

            if (!was_visited) {
                // Enque any unexplored edges, that will be queued up
                // according to their distance to the origin
                auto curr_dist = explored.find(this_pos)->second.dist + adj.second;
                explored.insert({adj.first, {this_pos, curr_dist}});
                queue.push({adj.first, curr_dist});
            }
        }
    }

    // See other problems for notes on reconstructing the path
    typename BOARD::PosVec path;
    auto tmp = explored.at(finish);
    while (tmp.parent != begin) {
        if (verbose) {
            std::cout << "Pushing " << tmp.parent << std::endl;
        }
        path.push_back(tmp.parent);
        tmp = explored.at(tmp.parent);
    }
    path.push_back(begin);
    std::reverse(path.begin(), path.end());
    path.push_back(finish);

    return path;
}
