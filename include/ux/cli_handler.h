#pragma once

#include <string>
#include <vector>

class CliHandler {
public:
    CliHandler();
    void printWelcome();
    std::string readInput();
    void printOutput(const std::string& output);
};
