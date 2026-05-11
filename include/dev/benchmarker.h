#pragma once

#include <string>
#include <chrono>

class Benchmarker {
public:
    Benchmarker(const std::string& name);
    void start();
    void stop();
    double getDurationMs() const;

private:
    std::string name;
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
};
