#pragma once

#include <string>

enum class Tone { NEUTRAL, EMPATHETIC, FORMAL, ASSERTIVE };

class ToneAnalyzer {
public:
    ToneAnalyzer();
    Tone detectTone(const std::string& input);
    std::string adjustResponse(const std::string& output, Tone target_tone);
};
