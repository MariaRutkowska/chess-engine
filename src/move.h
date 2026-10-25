#pragma once
#include <cstdint>
#include <string>
#include "board.h"
#include "types.h"

// Move packed into 16 bits:
// bits 0-5: from, bits 6-11: to, bits 12-13: promotion piece, bits 14-15: move type
using Move = std::uint16_t;
constexpr Move NO_MOVE = 0; // dla braku ruchu

enum MoveType : int { NORMAL = 0, PROMOTION = 1, EN_PASSANT = 2, CASTLING = 3 };

constexpr Move make_move(int from, int to, MoveType type = NORMAL, PieceType promo = KNIGHT) {
    return Move(from | (to << 6) | ((promo - KNIGHT) << 12) | (type << 14));
}

constexpr int move_from(Move m) { return m & 0b111111; }

constexpr int move_to(Move m) { return (m >> 6) & 0b111111;
}

constexpr PieceType move_promo(Move m) { return PieceType(((m >> 12) & 0b11) + KNIGHT); }

constexpr MoveType move_type(Move m) { return MoveType(((m >> 14) & 0b11));    
}

// Move in UCI notation, e.g. "e2e4" or "e7e8q"
inline std::string move_to_string(Move m) {
    std::string s = square_to_string(move_from(m)) + square_to_string(move_to(m));
    if (move_type(m) == PROMOTION) s += "nbrq"[move_promo(m) - KNIGHT];
    return s;
}