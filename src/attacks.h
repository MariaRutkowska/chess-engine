#pragma once
#include <array>
#include "types.h"

extern std::array<Bitboard, 64> KNIGHT_ATTACKS;
extern std::array<Bitboard, 64> KING_ATTACKS;

void init_attacks();