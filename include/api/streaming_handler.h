#pragma once

#include <string>
#include <functional>
#include <vector>

class StreamingHandler {
public:
    StreamingHandler();
    void streamResponse(const std::string& full_text, std::function<void(const std::string&)> chunk_callback);
};
