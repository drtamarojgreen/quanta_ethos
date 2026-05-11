#pragma once

#include <string>
#include <vector>

struct XAIReason {
    std::string criterion;
    bool passed;
    std::string detail;
};

class XAIProvider {
public:
    XAIProvider();
    void addReason(const XAIReason& reason);
    std::string generateExplanation() const;
    void clear();

private:
    std::vector<XAIReason> reasons;
};
