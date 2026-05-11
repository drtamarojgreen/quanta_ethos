#pragma once

#include <string>

class MessagingBridge {
public:
    MessagingBridge(const std::string& platform_name);
    void sendMessage(const std::string& user_id, const std::string& text);
    std::string receiveMessage();

private:
    std::string platform;
};
