#pragma once
#include <iostream>

// Runs the UCI (Universal Chess Interface) loop so the engine can talk to
// chess GUIs such as Cute Chess or Arena, and to testing tools like fastchess.
void uci_loop(std::istream& in = std::cin, std::ostream& out = std::cout);
