#pragma once

#include <string>

class CrashHandler {
public:
    CrashHandler();
    void setup();
    void reportCrash(const std::string& reason);
};
