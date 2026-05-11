#include "quantization_util.h"
#include <iostream>

QuantizationUtil::QuantizationUtil() {}

void QuantizationUtil::quantize8Bit(const std::string& input_path, const std::string& output_path) {
    std::cout << "Quantizing " << input_path << " to 8-bit integer precision. Output: " << output_path << "\n";
}

void QuantizationUtil::pruneModel(const std::string& input_path, float threshold) {
    std::cout << "Pruning weights in " << input_path << " below threshold " << threshold << "\n";
}
