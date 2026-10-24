#pragma once
#include <cstdint>

using Bitboard = std::uint64_t;

// Square indexing: a1 = 0, b1 = 1, ..., h1 = 7, a2 = 8, ..., h8 = 63
enum Square : int {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NO_SQUARE = 64
};

enum Color : int { WHITE = 0, BLACK = 1 };

enum PieceType : int { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

// Piece = color * 6 + type, so white pieces are 0..5 and black pieces 6..11
enum Piece : int {
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    NO_PIECE = 12
};

// Castling rights stored as 4 bits
enum CastlingRight : int {
    WHITE_OO  = 1,
    WHITE_OOO = 2,
    BLACK_OO  = 4,
    BLACK_OOO = 8
};

constexpr Piece make_piece(Color c, PieceType pt) { return Piece(c * 6 + pt); }
constexpr Color color_of(Piece p) { return Color(p / 6); }
constexpr PieceType type_of(Piece p) { return PieceType(p % 6); }

constexpr int file_of(int sq) { return sq & 7; }
constexpr int rank_of(int sq) { return sq >> 3; }
constexpr int make_square(int file, int rank) { return rank * 8 + file; }
