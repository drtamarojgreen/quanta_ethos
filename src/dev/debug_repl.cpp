#include "debug_repl.h"
#include <iostream>

DebugRepl::DebugRepl() {}

void DebugRepl::run() {
    std::cout << "Debug REPL started. Type 'exit' to quit.\n";
}

std::string DebugRepl::processCommand(const std::string& cmd) {
    if (cmd == "ping") return "pong";
    return "Unknown command: " + cmd;
}
