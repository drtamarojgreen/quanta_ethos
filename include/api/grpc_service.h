#pragma once

#include <string>

class GrpcService {
public:
    GrpcService();
    void start(int port);
    void stop();
    std::string handleRequest(const std::string& request_payload);
};
