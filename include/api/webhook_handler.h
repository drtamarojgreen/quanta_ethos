#pragma once

#include <string>

class WebhookHandler {
public:
    WebhookHandler();
    bool trigger(const std::string& url, const std::string& payload);
};
