#include <iostream>
#include "ethics_logic.h"

// Dummy llama.cpp connector (replace with real call)
std::string callLlamaCpp(const std::string& prompt) {
    return "This is a sample answer based on the prompt: " + prompt;
}

int main() {
    std::string userInput;

    std::cout << "PrismQuanta Ethical Assistant v1.0\n";
    std::cout << "Enter your question or instruction:\n> ";
    std::getline(std::cin, userInput);

    std::string ethicalPrompt = makeEthicalPrompt(userInput);
    std::string modelOutput = callLlamaCpp(ethicalPrompt);
    std::string reviewedOutput = applyTrustworthinessChecks(modelOutput);
    std::string reviewNotes = runSelfReview(modelOutput);
    float score = computeTrustScore(reviewNotes);

    std::cout << "\n[Response]\n" << reviewedOutput << "\n";
    std::cout << "[Trust Score: " << score << "]\n";

    logInteractionToJson(userInput, ethicalPrompt, modelOutput, reviewNotes, score);
    logInteractionToCsv(userInput, score);

    return 0;
}