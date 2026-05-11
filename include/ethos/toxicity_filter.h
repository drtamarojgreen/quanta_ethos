#pragma once

#include <string>
#include <vector>

class ToxicityFilter {
public:
    ToxicityFilter();
    void addBlockedWord(const std::string& word);
    bool isToxic(const std::string& text) const;
    std::string scrub(const std::string& text) const;

private:
    std::vector<std::string> blocked_words;
};
