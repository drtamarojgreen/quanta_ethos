#pragma once

#include <string>

class HwAccel {
public:
    enum class Backend { CPU, CUDA, METAL, VULKAN };
    HwAccel();
    void setBackend(Backend b);
    std::string getBackendName() const;
    bool isAvailable(Backend b) const;

private:
    Backend current;
};
