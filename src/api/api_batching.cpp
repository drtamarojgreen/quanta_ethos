#include "api_batching.h"

ApiBatching::ApiBatching() {}

std::vector<std::string> ApiBatching::processBatch(const std::vector<std::string>& prompts) {
    std::vector<std::string> responses;
    for (const auto& prompt : prompts) {
        responses.push_back("Batch response for: " + prompt);
    }
    return responses;
}
