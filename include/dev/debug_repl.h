#pragma once

#include <string>

class DebugRepl {
public:
    DebugRepl();
    void run();
    std::string processCommand(const std::string& command);
};
