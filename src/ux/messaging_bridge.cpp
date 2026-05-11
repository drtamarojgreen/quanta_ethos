#include "messaging_bridge.h"
#include <iostream>

MessagingBridge::MessagingBridge(const std::string& p) : platform(p) {}

void MessagingBridge::sendMessage(const std::string& id, const std::string& text) {
    std::cout << "[" << platform << "] Sending to " << id << ": " << text << "\n";
}

std::string MessagingBridge::receiveMessage() {
    return "Mock platform message";
}
