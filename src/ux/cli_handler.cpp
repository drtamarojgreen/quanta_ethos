#include "cli_handler.h"
#include <iostream>

CliHandler::CliHandler() {}

void CliHandler::printWelcome() {
    std::cout << "\033[1;32mWelcome to PrismQuanta CLI\033[0m\n";
}

std::string CliHandler::readInput() {
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);
    return input;
}

void CliHandler::printOutput(const std::string& out) {
    std::cout << "Assistant: " << out << "\n";
}
