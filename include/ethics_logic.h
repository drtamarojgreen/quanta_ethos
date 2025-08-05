#pragma once

#include <string>

std::string makeEthicalPrompt(const std::string& userInput);
std::string applyTrustworthinessChecks(const std::string& modelOutput);
std::string runSelfReview(const std::string& modelOutput);
float computeTrustScore(const std::string& reviewNotes);
void logInteractionToJson(const std::string& userInput, const std::string& ethicalPrompt, const std::string& modelOutput, const std::string& reviewNotes, float score);
void logInteractionToCsv(const std::string& userInput, float score);
