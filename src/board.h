#pragma once
#include <array>
#include <string>
#include "types.h"

inline const std::string START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

class Board {
public:
    Board();

    // Load a position from FEN. Returns false if the FEN is malformed.
    bool set_fen(const std::string& fen);
    std::string get_fen() const;

    // ASCII board for debugging (UCI command "d")
    std::string to_string() const;

    Piece piece_on(int sq) const { return mailbox_[sq]; }
    Bitboard pieces(Piece p) const { return piece_bb_[p]; }
    Bitboard occupancy(Color c) const { return color_bb_[c]; }
    Bitboard occupancy() const { return color_bb_[WHITE] | color_bb_[BLACK]; }

    Color side_to_move() const { return side_; }
    int castling_rights() const { return castling_; }
    int en_passant_square() const { return ep_square_; }
    int halfmove_clock() const { return halfmove_; }
    int fullmove_number() const { return fullmove_; }

private:
    void clear();
    void put_piece(Piece p, int sq);

    std::array<Bitboard, 12> piece_bb_{};   // one bitboard per piece kind
    std::array<Bitboard, 2> color_bb_{};    // all white / all black pieces
    std::array<Piece, 64> mailbox_{};       // piece on each square, for fast lookup

    Color side_ = WHITE;
    int castling_ = 0;
    int ep_square_ = NO_SQUARE;
    int halfmove_ = 0;
    int fullmove_ = 1;
};

std::string square_to_string(int sq);
int string_to_square(const std::string& s);
