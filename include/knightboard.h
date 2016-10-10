#pragma once

#include <array>
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <experimental/optional>
#include <sstream>
#include <fstream>

// I really like C++11's enum class, that cleanly namespaces enums
enum class BoardSquare {
    Clear,
    Water,
    Rock,
    Barrier,
    Teleport,
    Lava
};

// Templating wasn't strictly necessary here, but in principle I like having compile-time
// checked dimensions and static allocation when possible. std::array is great because it
// has the STL interface that we know and love (?!) from std::vector.

template<int BOARD_SIZE>
struct Board {
    struct Pos {
        /* holds a position in the board. */
        int x;
        int y;

        // Construct from row and col index
        Pos(int x_, int y_) : x(x_), y(y_) {}

        // Construct from a row-major integer index
        Pos(int index) {
                x = std::floor(index / BOARD_SIZE);
                y = index - (x * BOARD_SIZE);
        }

        // Copy constructor
        Pos(const Pos &other) : x(other.x), y(other.y) {}

        int as_int() const {
            return BOARD_SIZE * x + y;
        }

        bool operator==(const Pos &other) const {
            return (other.x == x && other.y == y);
        }

        bool operator!=(const Pos &other) const {
            return !(*this == other);
        }

        friend std::ostream &operator<<(std::ostream &out, const Pos &p) {
            out << "[x: " << p.x << " y: " << p.y << "]";
            return out;
        }
    };

    /* functor needed to use Pos as key in hashmaps */
    struct PosHasher {
        size_t operator()(const Pos &pos) const {
            return pos.as_int();
        }
    };

    Board() {
        // Clear up the board at construction time
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                b[i][j] = BoardSquare::Clear;
            }
        }
    }

    static constexpr int size = BOARD_SIZE;
    // Holds the square type data for the board
    std::array<std::array<BoardSquare, BOARD_SIZE>, BOARD_SIZE> b;
    // Optional pair of teleport portals (can easily be extended to multiple pairs)
    std::experimental::optional<std::pair<Pos, Pos>> teleports;

    // Some typedefs used in problems
    using PosVec = std::vector<Pos>;
    using GraphEdge = std::pair<Pos, int>;
    using GraphEdgeVec = std::vector<GraphEdge>;

    std::ostream &print(std::ostream &out,
                        std::experimental::optional<Pos> knight_pos = std::experimental::nullopt) const {

        static std::map<BoardSquare, char> square_codes = {
                {BoardSquare::Clear,    '.'},
                {BoardSquare::Water,    'W'},
                {BoardSquare::Rock,     'R'},
                {BoardSquare::Barrier,  'B'},
                {BoardSquare::Teleport, 'T'},
                {BoardSquare::Lava,     'L'}
        };

        // I usually use iterators, but need the indices here
        for (int i = 0; i < b.size(); i++) { // Iterating over rows
            for (int j = 0; j < b[i].size(); j++) { // Iterating over columns
                if (knight_pos && knight_pos->x == i && knight_pos->y == j) {
                    out << "\x1B[31mK \x1B[0m";
                } else {
                    std::cout << square_codes[b[i][j]] << " ";
                }
            }
            out << std::endl;
        }
        return out;
    }

    bool is_within_bounds(const Pos &pos) const {
        return ((pos.x >= 0) && (pos.x < BOARD_SIZE) && (pos.y >= 0) && (pos.y < BOARD_SIZE));
    }

    GraphEdgeVec adjacent_positions(const Pos &origin, const bool is_teleport_dest = false) const {
        // Returns adjacent (valid) positions from a starting point
        auto x = origin.x;
        auto y = origin.y;

        // In the teleport case, we return the set of positions adjancent to the
        // **destination** portal
        if (b[x][y] == BoardSquare::Teleport && !is_teleport_dest) {
            auto dest_portal = (origin == teleports->first) ? teleports->second : teleports->first;
            // Also add a flag to avoid infinite recursion
            return adjacent_positions(dest_portal, true);
        }

        PosVec candidates = {
                {x - 2, y - 1},
                {x - 2, y + 1},
                {x - 1, y - 2},
                {x - 1, y + 2},
                {x + 1, y - 2},
                {x + 1, y + 2},
                {x + 2, y - 1},
                {x + 2, y + 1}
        };

        GraphEdgeVec edges;

        for (const auto &pos: candidates) {
            if (!is_valid_step(origin, pos)) {
                continue;
            }

            // Assign weights to (valid) moves
            int weight = 1;
            switch (b[pos.x][pos.y]) {
                case BoardSquare::Water:
                    weight = 2;
                    break;
                case BoardSquare::Lava:
                    weight = 5;
                    break;
            }

            edges.emplace_back(pos, weight);
        }

        return edges;
    }

    bool is_valid_step(const Pos &begin, const Pos &end) const {
        // The knight moves "L-wise"
        auto abs_delta_x = std::abs(end.x - begin.x);
        auto abs_delta_y = std::abs(end.y - begin.y);

        bool is_teleport = (b[end.x][end.y] == b[begin.x][begin.y]) && (b[begin.x][begin.y] == BoardSquare::Teleport);

        bool is_right_shape = (abs_delta_x == 2 && abs_delta_y == 1) || (abs_delta_x == 1 && abs_delta_y == 2);
        bool is_allowed_end = (b[end.x][end.y] != BoardSquare::Rock) && (b[end.x][end.y] != BoardSquare::Barrier);

        bool is_allowed_cross = true;
        if (abs_delta_x == 2) { // Horizontal move
            for (int i = std::min(begin.x, end.x); i <= std::max(begin.x, end.x); i++) {
                if (b[i][begin.y] == BoardSquare::Barrier) {
                    is_allowed_cross = false;
                }
            }
        } else { // Vertical move
            for (int i = std::min(begin.y, end.y); i <= std::max(begin.y, end.y); i++) {
                if (b[begin.x][i] == BoardSquare::Barrier) {
                    is_allowed_cross = false;
                }
            }
        }

        // For level-1, this was enough
        // return is_within_bounds(end) && is_right_shape;

        return is_teleport || (
                is_within_bounds(end)
                && is_right_shape
                && is_allowed_end
                && is_allowed_cross);
    }

    void load_from_file() {
        std::string file_name = std::string(std::getenv("HOME")) + "/knightboard.txt";

        std::ifstream ifile(file_name);
        if (!ifile.is_open()) {
            std::cerr << "Failed opening file at " << file_name << std::endl;
            exit(1);
        }

        std::string line;
        std::vector<Pos> portals;
        int row_cnt = 0;

        while (std::getline(ifile, line)) {
            // Tokenize using stringstream
            std::istringstream iss(line);
            char sq;
            int col_cnt = 0;

            while (iss >> sq) {
                switch (sq) {
                    case '.':
                        b[row_cnt][col_cnt] = BoardSquare::Clear;
                        break;
                    case 'W':
                        b[row_cnt][col_cnt] = BoardSquare::Water;
                        break;
                    case 'R':
                        b[row_cnt][col_cnt] = BoardSquare::Rock;
                        break;
                    case 'B':
                        b[row_cnt][col_cnt] = BoardSquare::Barrier;
                        break;
                    case 'T':
                        b[row_cnt][col_cnt] = BoardSquare::Teleport;
                        portals.push_back({row_cnt, col_cnt});
                        break;
                    case 'L':
                        b[row_cnt][col_cnt] = BoardSquare::Lava;
                        break;
                }
                col_cnt += 1;
            }
            row_cnt += 1;
        }

        if (portals.size()) {
            if (portals.size() != 2) {
                throw std::runtime_error("Invalid number of teleport portals in the map");
            }
            teleports = std::make_pair(portals.at(0), portals.at(1));
        }
    }
};

template<int BOARD_SIZE>
std::ostream &operator<<(std::ostream &out, const Board<BOARD_SIZE> &board) {
    return board.print(out);
}

// Some convenience definitions
using Board8 = Board<8>;
using Pos8 = Board8::Pos;
using PosVec8 = Board8::PosVec;
using GraphEdgeVec8 = Board8::GraphEdgeVec;

using Board32 = Board<32>;
using Pos32 = Board32::Pos;
using PosVec32 = Board32::PosVec;