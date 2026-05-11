#include "toxicity_filter.h"
#include <algorithm>
#include <regex>

ToxicityFilter::ToxicityFilter() {
    blocked_words = {"harmful", "toxic", "offensive"};
}

void ToxicityFilter::addBlockedWord(const std::string& word) {
    blocked_words.push_back(word);
}

bool ToxicityFilter::isToxic(const std::string& text) const {
    std::string lower_text = text;
    std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), ::tolower);
    for (const auto& word : blocked_words) {
        if (lower_text.find(word) != std::string::npos) return true;
    }
    return false;
}

std::string ToxicityFilter::scrub(const std::string& text) const {
    std::string scrubbed = text;
    for (const auto& word : blocked_words) {
        std::regex pattern("(?i)" + word);
        scrubbed = std::regex_replace(scrubbed, pattern, "[REDACTED]");
    }
    return scrubbed;
}
