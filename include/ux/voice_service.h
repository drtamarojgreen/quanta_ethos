#pragma once

#include <string>

class VoiceService {
public:
    VoiceService();
    std::string speechToText(const std::vector<float>& audio_buffer);
    void textToSpeech(const std::string& text);
};
