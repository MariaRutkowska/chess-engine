#include "board.h"
#include "bitboard.h"

#include <sstream>
#include <string_view>

namespace {

constexpr std::string_view PIECE_CHARS = "PNBRQKpnbrqk";

Piece char_to_piece(char c) {
    auto pos = PIECE_CHARS.find(c);
    return pos == std::string_view::npos ? NO_PIECE : Piece(pos);
}

}  // namespace

std::string square_to_string(int sq) {
    if (sq == NO_SQUARE) return "-";
    return {char('a' + file_of(sq)), char('1' + rank_of(sq))};
}

int string_to_square(const std::string& s) {
    if (s.size() != 2 || s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[1] > '8')
        return NO_SQUARE;
    return make_square(s[0] - 'a', s[1] - '1');
}

Board::Board() { set_fen(START_FEN); }

void Board::clear() {
    piece_bb_.fill(0);
    color_bb_.fill(0);
    mailbox_.fill(NO_PIECE);
    side_ = WHITE;
    castling_ = 0;
    ep_square_ = NO_SQUARE;
    halfmove_ = 0;
    fullmove_ = 1;
}

void Board::put_piece(Piece p, int sq) {
    set_bit(piece_bb_[p], sq);
    set_bit(color_bb_[color_of(p)], sq);
    mailbox_[sq] = p;
}

bool Board::set_fen(const std::string& fen) {
    clear();
    std::istringstream ss(fen);
    std::string placement, side, castling, ep;
    if (!(ss >> placement >> side >> castling >> ep)) return false;

    // 1. Piece placement, from rank 8 down to rank 1
    int rank = 7, file = 0;
    for (char c : placement) {
        if (c == '/') {
            if (file != 8) return false;
            --rank;
            file = 0;
        } else if (c >= '1' && c <= '8') {
            file += c - '0';
        } else {
            Piece p = char_to_piece(c);
            if (p == NO_PIECE || file > 7 || rank < 0) return false;
            put_piece(p, make_square(file, rank));
            ++file;
        }
        if (file > 8) return false;
    }
    if (rank != 0 || file != 8) return false;

    // 2. Side to move
    if (side == "w") side_ = WHITE;
    else if (side == "b") side_ = BLACK;
    else return false;

    // 3. Castling rights
    if (castling != "-") {
        for (char c : castling) {
            switch (c) {
                case 'K': castling_ |= WHITE_OO; break;
                case 'Q': castling_ |= WHITE_OOO; break;
                case 'k': castling_ |= BLACK_OO; break;
                case 'q': castling_ |= BLACK_OOO; break;
                default: return false;
            }
        }
    }

    // 4. En passant square
    if (ep != "-") {
        ep_square_ = string_to_square(ep);
        if (ep_square_ == NO_SQUARE) return false;
    }

    // 5. Move counters are optional in many FENs
    if (!(ss >> halfmove_)) halfmove_ = 0;
    if (!(ss >> fullmove_)) fullmove_ = 1;
    return true;
}

std::string Board::get_fen() const {
    std::string fen;
    for (int rank = 7; rank >= 0; --rank) {
        int empty = 0;
        for (int file = 0; file < 8; ++file) {
            Piece p = mailbox_[make_square(file, rank)];
            if (p == NO_PIECE) {
                ++empty;
            } else {
                if (empty) fen += char('0' + empty);
                empty = 0;
                fen += PIECE_CHARS[p];
            }
        }
        if (empty) fen += char('0' + empty);
        if (rank > 0) fen += '/';
    }

    fen += side_ == WHITE ? " w " : " b ";

    std::string c;
    if (castling_ & WHITE_OO) c += 'K';
    if (castling_ & WHITE_OOO) c += 'Q';
    if (castling_ & BLACK_OO) c += 'k';
    if (castling_ & BLACK_OOO) c += 'q';
    fen += c.empty() ? "-" : c;

    fen += ' ' + square_to_string(ep_square_);
    fen += ' ' + std::to_string(halfmove_) + ' ' + std::to_string(fullmove_);
    return fen;
}

std::string Board::to_string() const {
    std::ostringstream out;
    out << "\n +---+---+---+---+---+---+---+---+\n";
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            Piece p = mailbox_[make_square(file, rank)];
            out << " | " << (p == NO_PIECE ? ' ' : PIECE_CHARS[p]);
        }
        out << " | " << rank + 1 << "\n +---+---+---+---+---+---+---+---+\n";
    }
    out << "   a   b   c   d   e   f   g   h\n\n";
    out << "Fen: " << get_fen() << "\n";
    return out.str();
}
