#include "uci.h"
#include "board.h"

#include <sstream>
#include <string>

namespace {

constexpr const char* ENGINE_NAME = "ChessEngine 0.1";
constexpr const char* ENGINE_AUTHOR = "mrska232";


void handle_position(Board& board, std::istringstream& ss, std::ostream& out) {
    std::string token, fen;
    ss >> token;

    if (token == "startpos") {
        fen = START_FEN;
        ss >> token;  
    } else if (token == "fen") {
        while (ss >> token && token != "moves") fen += token + ' ';
    } else {
        return;
    }

    if (!board.set_fen(fen)) {
        out << "info string invalid fen\n";
        board.set_fen(START_FEN);
        return;
    }

    if (token == "moves") {
        // TODO(move generation): parse each move (e.g. "e2e4", "e7e8q")
        // and apply it with board.make_move(...)
        out << "info string move list not supported yet\n";
    }
}

}  // namespace

void uci_loop(std::istream& in, std::ostream& out) {
    Board board;
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "uci") {
            out << "id name " << ENGINE_NAME << '\n'
                << "id author " << ENGINE_AUTHOR << '\n'
                << "uciok" << std::endl;
        } else if (cmd == "isready") {
            out << "readyok" << std::endl;
        } else if (cmd == "ucinewgame") {
            board.set_fen(START_FEN);
        } else if (cmd == "position") {
            handle_position(board, ss, out);
        } else if (cmd == "go") {
            // TODO(search): run the search and print the best move found
            out << "info string search not implemented yet\n"
                << "bestmove 0000" << std::endl;
        } else if (cmd == "d") {
            out << board.to_string() << std::flush;
        } else if (cmd == "quit") {
            break;
        } else if (!cmd.empty()) {
            out << "info string unknown command: " << cmd << std::endl;
        }
    }
}
