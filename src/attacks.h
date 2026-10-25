#pragma once
#include <array>
#include "types.h"

extern std::array<Bitboard, 64> KNIGHT_ATTACKS;
extern std::array<Bitboard, 64> KING_ATTACKS;
extern std::array<std::array<Bitboard, 64>, 2> PAWN_ATTACKS;
void init_attacks();

Bitboard rook_attacks(int sq, Bitboard occupied);
Bitboard bishop_attacks(int sq, Bitboard occupied);
Bitboard queen_attacks(int sq, Bitboard occupied);