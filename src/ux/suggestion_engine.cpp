#include "suggestion_engine.h"

SuggestionEngine::SuggestionEngine() {}

std::vector<std::string> SuggestionEngine::getSuggestions(const std::string& query) {
    return {"Tell me more about " + query, "Why is that?", "Give me an example."};
}
