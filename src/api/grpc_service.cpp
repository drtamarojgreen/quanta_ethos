#include "grpc_service.h"
#include <iostream>

GrpcService::GrpcService() {}

void GrpcService::start(int port) {
    std::cout << "Starting gRPC service on port " << port << "...\n";
}

void GrpcService::stop() {
    std::cout << "Stopping gRPC service...\n";
}

std::string GrpcService::handleRequest(const std::string& payload) {
    return "gRPC response to: " + payload;
}
