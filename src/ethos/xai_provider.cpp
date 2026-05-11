#include "xai_provider.h"
#include <sstream>

XAIProvider::XAIProvider() {}

void XAIProvider::addReason(const XAIReason& reason) {
    reasons.push_back(reason);
}

std::string XAIProvider::generateExplanation() const {
    std::ostringstream oss;
    oss << "Trust Score Explanation:\n";
    for (const auto& r : reasons) {
        oss << "- [" << (r.passed ? "PASS" : "FAIL") << "] " << r.criterion << ": " << r.detail << "\n";
    }
    return oss.str();
}

void XAIProvider::clear() {
    reasons.clear();
}
