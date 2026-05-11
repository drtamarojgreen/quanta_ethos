#pragma once

#include <string>

class SdkGenerator {
public:
    SdkGenerator();
    void generatePythonSdk(const std::string& output_path);
    void generateJsSdk(const std::string& output_path);
};
