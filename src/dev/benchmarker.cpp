#include "benchmarker.h"

Benchmarker::Benchmarker(const std::string& n) : name(n) {}

void Benchmarker::start() {
    start_time = std::chrono::steady_clock::now();
}

void Benchmarker::stop() {
    end_time = std::chrono::steady_clock::now();
}

double Benchmarker::getDurationMs() const {
    return std::chrono::duration<double, std::milli>(end_time - start_time).count();
}
