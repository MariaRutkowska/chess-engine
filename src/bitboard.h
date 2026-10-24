#pragma once
#include <bit>
#include "types.h"

// Basic bitboard helpers. Everything here compiles to one or two CPU instructions.

constexpr Bitboard square_bb(int sq) { return Bitboard(1) << sq; }

constexpr bool test_bit(Bitboard b, int sq) { return (b >> sq) & 1; }
constexpr void set_bit(Bitboard& b, int sq) { b |= square_bb(sq); }
constexpr void clear_bit(Bitboard& b, int sq) { b &= ~square_bb(sq); }

// Number of set bits (e.g. how many white pawns are left)
constexpr int popcount(Bitboard b) { return std::popcount(b); }

// Index of the least significant set bit (b must not be 0)
constexpr int lsb(Bitboard b) { return std::countr_zero(b); }

// Returns the lowest set square and removes it from b.
// Typical loop: while (b) { int sq = pop_lsb(b); ... }
constexpr int pop_lsb(Bitboard& b) {
    int sq = lsb(b);
    b &= b - 1;
    return sq;
}

constexpr Bitboard FILE_A = 0x0101010101010101ULL;
constexpr Bitboard FILE_H = FILE_A << 7;
constexpr Bitboard RANK_1 = 0xFFULL;
constexpr Bitboard RANK_8 = RANK_1 << 56;
