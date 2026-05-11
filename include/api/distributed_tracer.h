#pragma once

#include <string>

class DistributedTracer {
public:
    DistributedTracer();
    std::string startSpan(const std::string& operation_name);
    void endSpan(const std::string& span_id);
    void injectContext(const std::string& span_id, std::string& carrier);
};
