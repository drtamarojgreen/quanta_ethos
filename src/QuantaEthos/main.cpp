#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "ethics_logic.h"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " \"<command_to_validate>\"" << std::endl;
        return 1;
    }

    std::string command_to_validate = argv[1];

    // Use the existing ethics logic functions to validate the command
    std::string review_notes = runSelfReview(command_to_validate);
    float trust_score = computeTrustScore(review_notes);

    std::string decision = "deny";
    std::string reason = "Default reason: trust score too low.";

    // Define a threshold for approval. This should be configurable later.
    const float APPROVAL_THRESHOLD = 0.5f;

    if (trust_score >= APPROVAL_THRESHOLD) {
        decision = "approve";
        reason = "Trust score is above the threshold.";
    } else {
        if (review_notes.find("rm -rf") != std::string::npos) {
            reason = "High-risk command pattern 'rm -rf' detected.";
        }
    }

    json response = {
        {"decision", decision},
        {"reason", reason},
        {"trust_score", trust_score}
    };

    std::cout << response.dump(4) << std::endl;

    return 0;
}
