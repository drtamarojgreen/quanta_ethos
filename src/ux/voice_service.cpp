#include "voice_service.h"
#include <iostream>

VoiceService::VoiceService() {}

std::string VoiceService::speechToText(const std::vector<float>& audio) {
    return "recognized text";
}

void VoiceService::textToSpeech(const std::string& text) {
    std::cout << "Synthesizing voice: " << text << "\n";
}
