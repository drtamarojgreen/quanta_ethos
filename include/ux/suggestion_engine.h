#pragma once

#include <string>
#include <vector>

class SuggestionEngine {
public:
    SuggestionEngine();
    std::vector<std::string> getSuggestions(const std::string& last_query);
};
