#include "attacks.h"
#include "bitboard.h"

std::array<Bitboard, 64> KNIGHT_ATTACKS;
std::array<Bitboard, 64> KING_ATTACKS;

constexpr Bitboard FILE_B = FILE_A << 1;
constexpr Bitboard FILE_G = FILE_A << 6;

static Bitboard knight_attacks(int sq) {
    Bitboard b = square_bb(sq);
    Bitboard a = 0;
    a |= (b << 17) & ~FILE_A;
    a |= (b << 15) & ~FILE_H;
    a |= (b << 10) & ~(FILE_A | FILE_B);
    // TODO: dopisz pozostałe 5 linijek według tabeli
    a |= (b << 6) & ~(FILE_G | FILE_H);
    a |= (b >> 17) & ~ FILE_H;
    a |= (b >> 15) & ~ FILE_A;
    a |= (b >> 10) & ~(FILE_G | FILE_H);
    a |= (b >> 6) & ~(FILE_A | FILE_B);
    return a;
}

static Bitboard king_attacks(int sq) {

    Bitboard b = square_bb(sq);
    Bitboard a = 0;
    a |= (b << 8);
    a |= (b >> 8);
    a |= (b << 1) & ~FILE_A;
    a |= (b >> 1) & ~FILE_H;
    a |= (b << 9) & ~FILE_A;
    a |= (b << 7) & ~FILE_H;
    a |= (b >> 7) & ~FILE_A;
    a |= (b >> 9) & ~FILE_H;
}

void init_attacks() {
    for (int sq = 0; sq < 64; ++sq) {
        KNIGHT_ATTACKS[sq] = knight_attacks(sq);
        KING_ATTACKS[sq] = king_attacks(sq);
    }
}