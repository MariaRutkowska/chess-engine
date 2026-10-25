#pragma once
#include <array>
#include "board.h"
#include "move.h"

struct MoveList {
    std::array<Move, 256> moves{};
    int count = 0;

    void add(Move m) { moves[count++] = m; }
};

void generate_moves(const Board& board, MoveList& list);