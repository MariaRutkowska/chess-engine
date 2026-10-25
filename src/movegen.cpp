#include "movegen.h"
#include "attacks.h"
#include "bitboard.h"

static void generate_knight_moves(const Board &board, MoveList &list)
{
    Color us = board.side_to_move();
    Bitboard own = board.occupancy(us);
    Bitboard knights = board.pieces(make_piece(us, KNIGHT));

    while (knights)
    {
        int from = pop_lsb(knights);
        Bitboard targets = KNIGHT_ATTACKS[from] & ~own;
        while (targets)
        {
            int to = pop_lsb(targets);
            list.add(make_move(from, to));
        }
    }
}

static void generate_king_moves(const Board &board, MoveList &list)
{
    Color us = board.side_to_move();
    Bitboard own = board.occupancy(us);
    Bitboard kings = board.pieces(make_piece(us, KING));

    while (kings)
    {
        int from = pop_lsb(kings);
        Bitboard targets = KING_ATTACKS[from] & ~own;
        while (targets)
        {
            int to = pop_lsb(targets);
            list.add(make_move(from, to));
        }
    }
}

static void generate_rook_moves(const Board &board, MoveList &list)
{
    Color us = board.side_to_move();
    Bitboard own = board.occupancy(us);
    Bitboard occupied = board.occupancy();
    Bitboard rooks = board.pieces(make_piece(us, ROOK));

    while (rooks)
    {
        int from = pop_lsb(rooks);
        Bitboard targets = rook_attacks(from, occupied) & ~own;
        while (targets)
        {
            int to = pop_lsb(targets);
            list.add(make_move(from, to));
        }
    }
}

static void generate_bishop_moves(const Board &board, MoveList &list)
{
    Color us = board.side_to_move();
    Bitboard own = board.occupancy(us);
    Bitboard occupied = board.occupancy();
    Bitboard bishops = board.pieces(make_piece(us, BISHOP));

    while (bishops)
    {
        int from = pop_lsb(bishops);
        Bitboard targets = bishop_attacks(from, occupied) & ~own;
        while (targets)
        {
            int to = pop_lsb(targets);
            list.add(make_move(from, to));
        }
    }
}

static void generate_queen_moves(const Board &board, MoveList &list)
{
    Color us = board.side_to_move();
    Bitboard own = board.occupancy(us);
    Bitboard occupied = board.occupancy();
    Bitboard queens = board.pieces(make_piece(us, QUEEN));

    while (queens)
    {
        int from = pop_lsb(queens);
        Bitboard targets = queen_attacks(from, occupied) & ~own;
        while (targets)
        {
            int to = pop_lsb(targets);
            list.add(make_move(from, to));
        }
    }
}

static void add_pawn_move(MoveList &list, int from, int to, int promo_rank)
{
    if (rank_of(to) == promo_rank)
    {
        list.add(make_move(from, to, PROMOTION, QUEEN));
        list.add(make_move(from, to, PROMOTION, ROOK));
        list.add(make_move(from, to, PROMOTION, BISHOP));
        list.add(make_move(from, to, PROMOTION, KNIGHT));
    }
    else
    {
        list.add(make_move(from, to));
    }
}

static void generate_pawn_moves(const Board &board, MoveList &list)
{
    Color us = board.side_to_move();
    Color them = (us == WHITE) ? BLACK : WHITE;
    Bitboard occupied = board.occupancy();
    Bitboard enemy = board.occupancy(them);
    Bitboard pawns = board.pieces(make_piece(us, PAWN));

    int up = (us == WHITE) ? 8 : -8;
    int start_rank = (us == WHITE) ? 1 : 6;
    int promo_rank = (us == WHITE) ? 7 : 0;
    int ep = board.en_passant_square();

    while (pawns)
    {
        int from = pop_lsb(pawns);

        int to = from + up;
        if (!test_bit(occupied, to))
        {
            add_pawn_move(list, from, to, promo_rank);

            int to2 = to + up;
            if (rank_of(from) == start_rank && !test_bit(occupied, to2))
            {
                list.add(make_move(from, to2));
            }
        }

        Bitboard targets = PAWN_ATTACKS[us][from] & enemy;
        while (targets)
        {
            int cap = pop_lsb(targets);
            add_pawn_move(list, from, cap, promo_rank);
        }

        if (ep != NO_SQUARE && (PAWN_ATTACKS[us][from] & square_bb(ep)))
        {
            list.add(make_move(from, ep, EN_PASSANT));
        }
    }
}

void generate_moves(const Board &board, MoveList &list)
{
    generate_knight_moves(board, list);
    generate_king_moves(board, list);
    generate_rook_moves(board, list);
    generate_bishop_moves(board, list);
    generate_queen_moves(board, list);
    generate_pawn_moves(board, list);
}
