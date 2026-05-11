#include "sdk_generator.h"
#include <iostream>

SdkGenerator::SdkGenerator() {}

void SdkGenerator::generatePythonSdk(const std::string& path) {
    std::cout << "Generating Python SDK at " << path << "...\n";
}

void SdkGenerator::generateJsSdk(const std::string& path) {
    std::cout << "Generating JavaScript SDK at " << path << "...\n";
}
