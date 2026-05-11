#pragma once

#include <vector>

class VectorizedOps {
public:
    VectorizedOps();
    void addArrays(const float* a, const float* b, float* res, size_t size);
};
