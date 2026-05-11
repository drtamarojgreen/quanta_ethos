#pragma once

#include <string>

class QuantizationUtil {
public:
    QuantizationUtil();
    void quantize8Bit(const std::string& input_path, const std::string& output_path);
    void pruneModel(const std::string& input_path, float threshold);
};
