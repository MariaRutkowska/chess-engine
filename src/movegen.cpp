#include "movegen.h"
#include "attacks.h"
#include "bitboard.h"

static void generate_knight_moves(const Board& board, MoveList& list) {
    Color us = board.side_to_move();
    Bitboard own = board.occupancy(us);
    Bitboard knights = board.pieces(make_piece(us, KNIGHT));

    while (knights) {
        int from = pop_lsb(knights);
        Bitboard targets = KNIGHT_ATTACKS[from] & ~own;
        while (targets) {
            int to = pop_lsb(targets);
            list.add(make_move(from, to));
        }
    }
}

static void generate_king_moves(const Board& board, MoveList& list) {
    Color us = board.side_to_move();
    Bitboard own = board.occupancy(us);
    Bitboard kings = board.pieces(make_piece(us, KING));

    while(kings) {
        int from = pop_lsb(kings);
        Bitboard targets = KING_ATTACKS[from] & ~own;
        while(targets) {
            int to = pop_lsb(targets);
            list.add(make_move(from, to));
        }
    }
}

void generate_moves(const Board& board, MoveList& list) {
    generate_knight_moves(board, list);
    generate_king_moves(board, list);
}