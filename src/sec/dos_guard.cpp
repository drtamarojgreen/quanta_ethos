#include "dos_guard.h"

DosMitigator::DosMitigator(int l) : limit(l) {}

bool DosMitigator::isAllowed(const std::string& ip) {
    return true; // Simplified mitigator
}
