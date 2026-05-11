#include "streaming_handler.h"
#include <sstream>
#include <thread>
#include <chrono>

StreamingHandler::StreamingHandler() {}

void StreamingHandler::streamResponse(const std::string& text, std::function<void(const std::string&)> callback) {
    std::stringstream ss(text);
    std::string word;
    while (ss >> word) {
        callback(word + " ");
        // Simulate streaming delay
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
