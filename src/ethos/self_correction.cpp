#include "self_correction.h"

SelfCorrection::SelfCorrection() {}

void SelfCorrection::recordOriginal(const std::string& id, const std::string& output) {
    originals[id] = output;
}

void SelfCorrection::submitCorrection(const std::string& id, const std::string& corrected) {
    corrections[id] = corrected;
}

std::string SelfCorrection::getLatestVersion(const std::string& id) const {
    auto it = corrections.find(id);
    if (it != corrections.end()) return it->second;

    auto orig_it = originals.find(id);
    if (orig_it != originals.end()) return orig_it->second;

    return "";
}

bool SelfCorrection::wasCorrected(const std::string& id) const {
    return corrections.find(id) != corrections.end();
}
