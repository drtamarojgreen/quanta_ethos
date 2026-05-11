#include "hw_accel.h"

HwAccel::HwAccel() : current(Backend::CPU) {}

void HwAccel::setBackend(Backend b) {
    current = b;
}

std::string HwAccel::getBackendName() const {
    switch(current) {
        case Backend::CPU: return "CPU";
        case Backend::CUDA: return "CUDA";
        case Backend::METAL: return "METAL";
        case Backend::VULKAN: return "VULKAN";
        default: return "Unknown";
    }
}

bool HwAccel::isAvailable(Backend b) const {
    if (b == Backend::CPU) return true;
    return false; // Hardware specific checks would go here
}
