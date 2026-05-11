#include "bias_scanner.h"

BiasScanner::BiasScanner() {}

BiasResult BiasScanner::scan(const std::string& text) {
    // Simple mock detection
    if (text.find("exclusive") != std::string::npos) {
        return {true, "Inclusivity", "Consider using 'inclusive' instead."};
    }
    return {false, "", ""};
}

std::string BiasScanner::mitigate(const std::string& text, const BiasResult& bias) {
    if (!bias.detected) return text;
    return "Mitigated: " + text;
}
