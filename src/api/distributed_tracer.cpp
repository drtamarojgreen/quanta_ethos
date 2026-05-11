#include "distributed_tracer.h"
#include <iostream>

DistributedTracer::DistributedTracer() {}

std::string DistributedTracer::startSpan(const std::string& op) {
    std::string id = "span-" + op + "-123";
    std::cout << "Starting trace span: " << id << "\n";
    return id;
}

void DistributedTracer::endSpan(const std::string& id) {
    std::cout << "Ending trace span: " << id << "\n";
}

void DistributedTracer::injectContext(const std::string& id, std::string& carrier) {
    carrier += "x-trace-id: " + id;
}
