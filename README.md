# ChessEngine

A UCI chess engine written from scratch in modern C++20, built around bitboards and
developed with a focus on correctness testing and statistically sound strength measurement.

> Work in progress. See the roadmap below for current status. Done as fun side project :) Overall plan is used with a help of Claude AI, however programming is done by a human being (me).

## Features

- [x] Bitboard board representation (12 piece bitboards + mailbox)
- [x] FEN parsing and serialization
- [x] UCI protocol skeleton (`uci`, `isready`, `position`, `d`, `quit`)
- [x] Unit tests and CI on Linux and macOS
- [ ] Legal move generation (magic bitboards for sliders)
- [ ] Perft validation against standard test positions
- [ ] Alpha-beta search with iterative deepening
- [ ] Quiescence search
- [ ] Transposition table with Zobrist hashing
- [ ] Move ordering (MVV-LVA, killer moves, history heuristic)
- [ ] Tapered evaluation (piece-square tables)
- [ ] SPRT-tested improvements with Elo tracking
- [ ] Texel tuning of evaluation weights

## Build

Requirements: CMake 3.20+ and a C++20 compiler (Clang or GCC).

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Run the tests:

```bash
ctest --test-dir build --output-on-failure
```

## Usage

The engine speaks the [UCI protocol](https://www.wbec-ridderkerk.nl/html/UCIProtocol.html),
so it works with any UCI chess GUI (e.g. Cute Chess, Arena).

```text
$ ./build/engine
uci
id name ChessEngine 0.1
uciok
position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1
d
```

## Project structure

```text
src/
  types.h       core types: squares, pieces, colors, castling rights
  bitboard.h    bit manipulation helpers (popcount, lsb, pop_lsb)
  board.h/.cpp  position representation and FEN handling
  uci.h/.cpp    UCI protocol loop
  main.cpp      entry point
tests/
  test_board.cpp
```

## Testing methodology

*(Planned)* Every change to search or evaluation will be validated with
SPRT (Sequential Probability Ratio Test) self-play matches, and the results
(Elo difference with error bounds) will be recorded here.

| Version | Change | Elo vs previous | Games |
|---------|--------|-----------------|-------|
| 0.1     | Board representation, FEN, UCI skeleton | n/a | n/a |

## License

MIT
