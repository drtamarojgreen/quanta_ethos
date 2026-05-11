#include "webhook_handler.h"
#include <iostream>

WebhookHandler::WebhookHandler() {}

bool WebhookHandler::trigger(const std::string& url, const std::string& payload) {
    std::cout << "Triggering webhook: POST " << url << " with payload: " << payload << "\n";
    // Simulation of success
    return true;
}
