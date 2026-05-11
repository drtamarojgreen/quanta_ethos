#pragma once

#include <string>

class DosMitigator {
public:
    DosMitigator(int max_rate);
    bool isAllowed(const std::string& ip_address);

private:
    int limit;
};
