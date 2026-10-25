// tests written by Claude AI
#include "bitboard.h"
#include "board.h"
#include "attacks.h"
#include "move.h"
#include "movegen.h"

#include <iostream>
#include <string>
#include <vector>

static int failures = 0;

#define CHECK(expr)                                                        \
    do {                                                                   \
        if (!(expr)) {                                                     \
            std::cerr << __FILE__ << ':' << __LINE__ << "  FAILED: " #expr \
                      << '\n';                                             \
            ++failures;                                                    \
        }                                                                  \
    } while (0)

static void test_bitboard_helpers() {
    Bitboard b = 0;
    set_bit(b, E4);
    set_bit(b, A1);
    CHECK(popcount(b) == 2);
    CHECK(lsb(b) == A1);
    CHECK(pop_lsb(b) == A1);
    CHECK(b == square_bb(E4));
    clear_bit(b, E4);
    CHECK(b == 0);
}

static void test_squares() {
    CHECK(square_to_string(A1) == "a1");
    CHECK(square_to_string(H8) == "h8");
    CHECK(string_to_square("e4") == E4);
    CHECK(string_to_square("z9") == NO_SQUARE);
}

static void test_start_position() {
    Board b;
    CHECK(b.side_to_move() == WHITE);
    CHECK(popcount(b.occupancy()) == 32);
    CHECK(popcount(b.pieces(W_PAWN)) == 8);
    CHECK(b.piece_on(E1) == W_KING);
    CHECK(b.piece_on(D8) == B_QUEEN);
    CHECK(b.castling_rights() == (WHITE_OO | WHITE_OOO | BLACK_OO | BLACK_OOO));
    CHECK(b.en_passant_square() == NO_SQUARE);
}

static void test_fen_round_trip() {
    const std::vector<std::string> fens = {
        START_FEN,
        // "Kiwipete" - standard perft test position
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
    };
    for (const auto& fen : fens) {
        Board b;
        CHECK(b.set_fen(fen));
        CHECK(b.get_fen() == fen);
    }
}

static void test_invalid_fen() {
    Board b;
    CHECK(!b.set_fen(""));
    CHECK(!b.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP w KQkq - 0 1"));  // 7 ranks
    CHECK(!b.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1"));
    CHECK(!b.set_fen("rnbqkbnr/pppppppp/9/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
}

static void test_knight_attacks() {
    init_attacks();
    CHECK(popcount(KNIGHT_ATTACKS[A1]) == 2);
    CHECK(popcount(KNIGHT_ATTACKS[H1]) == 2);
    CHECK(popcount(KNIGHT_ATTACKS[E4]) == 8);
    CHECK(popcount(KNIGHT_ATTACKS[H8]) == 2);
    CHECK(popcount(KNIGHT_ATTACKS[B1]) == 3);
    CHECK(KNIGHT_ATTACKS[A1] == (square_bb(B3) | square_bb(C2)));
    CHECK(KNIGHT_ATTACKS[G1] == (square_bb(E2) | square_bb(F3) | square_bb(H3)));
}

static void test_king_attacks() {
    init_attacks();
    CHECK(popcount(KING_ATTACKS[A1]) == 3);
    CHECK(popcount(KING_ATTACKS[E1]) == 5);
    CHECK(popcount(KING_ATTACKS[E4]) == 8);
    CHECK(popcount(KING_ATTACKS[H8]) == 3);
    CHECK(popcount(KING_ATTACKS[H4]) == 5);
    CHECK(KING_ATTACKS[A1] == (square_bb(A2) | square_bb(B1) | square_bb(B2)));
}

static void test_pawn_attacks() {
    init_attacks();
    CHECK(PAWN_ATTACKS[WHITE][E4] == (square_bb(D5) | square_bb(F5)));
    CHECK(PAWN_ATTACKS[WHITE][A2] == square_bb(B3));
    CHECK(PAWN_ATTACKS[WHITE][H2] == square_bb(G3));
    CHECK(PAWN_ATTACKS[BLACK][E5] == (square_bb(D4) | square_bb(F4)));
    CHECK(PAWN_ATTACKS[BLACK][A7] == square_bb(B6));
    CHECK(PAWN_ATTACKS[BLACK][H7] == square_bb(G6));
}

static void test_rook_attacks() {
    
    CHECK(popcount(rook_attacks(A1, 0)) == 14);
    CHECK(popcount(rook_attacks(E4, 0)) == 14);
    CHECK(popcount(rook_attacks(H8, 0)) == 14);

    
    Bitboard occ = square_bb(A3);
    CHECK(popcount(rook_attacks(A1, occ)) == 9);
    CHECK(test_bit(rook_attacks(A1, occ), A3));
    CHECK(!test_bit(rook_attacks(A1, occ), A4));


    occ = square_bb(E5) | square_bb(E3) | square_bb(D4) | square_bb(F4);
    CHECK(rook_attacks(E4, occ) == occ);
}

static void test_bishop_attacks() {
    // pusta plansza
    CHECK(popcount(bishop_attacks(A1, 0)) == 7);
    CHECK(popcount(bishop_attacks(E4, 0)) == 13);
    CHECK(popcount(bishop_attacks(H8, 0)) == 7);
    CHECK(popcount(bishop_attacks(A4, 0)) == 7);

    // goniec na e4 otoczony przeszkodami na d5, f5, d3, f3
    Bitboard occ = square_bb(D5) | square_bb(F5) | square_bb(D3) | square_bb(F3);
    CHECK(bishop_attacks(E4, occ) == occ);

    // goniec na c1, przeszkoda na e3: d2, e3 oraz b2, a3
    occ = square_bb(E3);
    CHECK(bishop_attacks(C1, occ) ==
          (square_bb(D2) | square_bb(E3) | square_bb(B2) | square_bb(A3)));
}

static void test_queen_attacks() {
    CHECK(popcount(queen_attacks(E4, 0)) == 27);
    CHECK(popcount(queen_attacks(A1, 0)) == 21);
}

static void test_moves() {
    Move m = make_move(E2, E4);
    CHECK(move_from(m) == E2);
    CHECK(move_to(m) == E4);
    CHECK(move_type(m) == NORMAL);
    CHECK(move_to_string(m) == "e2e4");

    Move p = make_move(E7, E8, PROMOTION, QUEEN);
    CHECK(move_from(p) == E7);
    CHECK(move_to(p) == E8);
    CHECK(move_type(p) == PROMOTION);
    CHECK(move_promo(p) == QUEEN);
    CHECK(move_to_string(p) == "e7e8q");

    Move c = make_move(E1, G1, CASTLING);
    CHECK(move_type(c) == CASTLING);
    CHECK(move_to(c) == G1);
    CHECK(move_to_string(c) == "e1g1");
}


static int count_moves(const std::string& fen) {
    Board b;
    b.set_fen(fen);
    MoveList list;
    generate_moves(b, list);
    return list.count;
}

static void test_movegen_knight_king() {
    init_attacks();
    // sam skoczek na e4
    CHECK(count_moves("8/8/8/8/4N3/8/8/8 w - - 0 1") == 8);
    // własny pion na f6 blokuje jedno pole skoczka (pion ma 1 ruch: f7)
    CHECK(count_moves("8/8/5P2/8/4N3/8/8/8 w - - 0 1") == 8);
    // czarny pion na f6 można zbić
    CHECK(count_moves("8/8/5p2/8/4N3/8/8/8 w - - 0 1") == 8);
    // sam król na e4
    CHECK(count_moves("8/8/8/8/4K3/8/8/8 w - - 0 1") == 8);
    // król w rogu
    CHECK(count_moves("8/8/8/8/8/8/8/K7 w - - 0 1") == 3);
    // pozycja startowa: 4 ruchy skoczków + 16 ruchów pionów
    CHECK(count_moves(START_FEN) == 20);
    // to samo dla czarnych
    CHECK(count_moves("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") == 20);
}

static void test_movegen_sliders() {
    init_attacks();
    // sama wieża na e4
    CHECK(count_moves("8/8/8/8/4R3/8/8/8 w - - 0 1") == 14);
    // sam goniec na e4
    CHECK(count_moves("8/8/8/8/4B3/8/8/8 w - - 0 1") == 13);
    // sam hetman na e4
    CHECK(count_moves("8/8/8/8/4Q3/8/8/8 w - - 0 1") == 27);
    // wieża: a2 i bicie na b1; pion: a4
    CHECK(count_moves("8/8/8/8/8/P7/8/Rp6 w - - 0 1") == 3);
    // pozycja startowa
    CHECK(count_moves(START_FEN) == 20);
}

static void test_movegen_pawns() {
    init_attacks();
    // pojedynczy biały pion na e2: e3 i e4
    CHECK(count_moves("8/8/8/8/8/8/4P3/8 w - - 0 1") == 2);
    // pion na e2 zablokowany przez figurę na e3: 0 ruchów
    CHECK(count_moves("8/8/8/8/8/4p3/4P3/8 w - - 0 1") == 0);
    // figura na e4: tylko e3 (nie można przeskoczyć)
    CHECK(count_moves("8/8/8/8/4p3/8/4P3/8 w - - 0 1") == 1);
    // pion na e3 (nie na rzędzie startowym): tylko e4
    CHECK(count_moves("8/8/8/8/8/4P3/8/8 w - - 0 1") == 1);
    // bicia: pion na e4, czarne piony na d5 i f5: e5, exd5, exf5
    CHECK(count_moves("8/8/8/3p1p2/4P3/8/8/8 w - - 0 1") == 3);
    // czarny pion na e7: e6 i e5
    CHECK(count_moves("8/4p3/8/8/8/8/8/8 b - - 0 1") == 2);
    // czarne bicia: pion na e5, białe na d4 i f4: e4, exd4, exf4
    CHECK(count_moves("8/8/8/4p3/3P1P2/8/8/8 b - - 0 1") == 3);
    // pozycja startowa: 4 ruchy skoczków + 16 ruchów pionów
    CHECK(count_moves(START_FEN) == 20);
    CHECK(count_moves("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1") == 20);
}


static void test_movegen_promotions() {
    init_attacks();
    // biały pion na e7, e8 puste: 4 promocje
    CHECK(count_moves("8/4P3/8/8/8/8/8/8 w - - 0 1") == 4);
    // e8 puste, czarne wieże na d8 i f8: 4 ruchem + 4 biciem d8 + 4 biciem f8
    CHECK(count_moves("3r1r2/4P3/8/8/8/8/8/8 w - - 0 1") == 12);
    // e8 zablokowane, ale można bić na d8: 4
    CHECK(count_moves("3rr3/4P3/8/8/8/8/8/8 w - - 0 1") == 4);
    // czarny pion na e2: 4 promocje
    CHECK(count_moves("8/8/8/8/8/8/4p3/8 b - - 0 1") == 4);

    // pierwszy ruch na liście to promocja na hetmana
    Board b;
    b.set_fen("8/4P3/8/8/8/8/8/8 w - - 0 1");
    MoveList list;
    generate_moves(b, list);
    CHECK(move_type(list.moves[0]) == PROMOTION);
    CHECK(move_promo(list.moves[0]) == QUEEN);
    CHECK(move_to_string(list.moves[0]) == "e7e8q");
}

static void test_movegen_en_passant() {
    init_attacks();
    // biały e5, czarny d5, pole ep d6: e6 + exd6 w przelocie
    CHECK(count_moves("8/8/8/3pP3/8/8/8/8 w - d6 0 1") == 2);
    // ta sama pozycja bez prawa en passant: tylko e6
    CHECK(count_moves("8/8/8/3pP3/8/8/8/8 w - - 0 1") == 1);
    // czarny e4, biały d4, pole ep d3: e3 + exd3
    CHECK(count_moves("8/8/8/8/3Pp3/8/8/8 b - d3 0 1") == 2);
    // pion za daleko od pola ep: tylko zwykły ruch
    CHECK(count_moves("8/8/8/3p4/8/4P3/8/8 w - d6 0 1") == 1);

    // ruch en passant ma właściwy rodzaj i zapis
    Board b;
    b.set_fen("8/8/8/3pP3/8/8/8/8 w - d6 0 1");
    MoveList list;
    generate_moves(b, list);
    bool found = false;
    for (int i = 0; i < list.count; ++i) {
        if (move_type(list.moves[i]) == EN_PASSANT) {
            found = true;
            CHECK(move_to_string(list.moves[i]) == "e5d6");
        }
    }
    CHECK(found);
}


int main() {
    test_bitboard_helpers();
    test_squares();
    test_start_position();
    test_fen_round_trip();
    test_invalid_fen();
    test_pawn_attacks();
    test_king_attacks();
    test_knight_attacks();
    test_rook_attacks();
    test_bishop_attacks();
    test_queen_attacks();
    test_moves();
    test_movegen_knight_king();
    test_movegen_sliders();
    test_movegen_pawns();
    test_movegen_promotions();
    test_movegen_en_passant();

    if (failures) {
        std::cerr << failures << " check(s) failed\n";
        return 1;
    }
    std::cout << "All tests passed\n";
    return 0;
}
