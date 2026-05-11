#include "crash_mgr.h"
#include <iostream>

CrashHandler::CrashHandler() {}

void CrashHandler::setup() {
    std::cout << "Setting up crash handler hooks...\n";
}

void CrashHandler::reportCrash(const std::string& reason) {
    std::cerr << "CRASH DETECTED: " << reason << "\n";
}
