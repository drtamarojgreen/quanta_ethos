#include "tone_analyzer.h"

ToneAnalyzer::ToneAnalyzer() {}

Tone ToneAnalyzer::detectTone(const std::string& input) {
    if (input.find("please") != std::string::npos) return Tone::EMPATHETIC;
    if (input.find("must") != std::string::npos) return Tone::ASSERTIVE;
    return Tone::NEUTRAL;
}

std::string ToneAnalyzer::adjustResponse(const std::string& output, Tone target_tone) {
    switch(target_tone) {
        case Tone::EMPATHETIC: return "I understand. " + output;
        case Tone::FORMAL: return "Kindly be advised: " + output;
        case Tone::ASSERTIVE: return "Note: " + output;
        default: return output;
    }
}
