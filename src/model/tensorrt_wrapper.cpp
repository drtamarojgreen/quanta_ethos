#include "tensorrt_wrapper.h"
#include <iostream>

TensorRTWrapper::TensorRTWrapper() {}

bool TensorRTWrapper::optimizeModel(const std::string& model_path, const std::string& output_engine_path) {
    std::cout << "Optimizing " << model_path << " using TensorRT. Saving to " << output_engine_path << "...\n";
    return true;
}

std::string TensorRTWrapper::runInference(const std::string& input_data) {
    return "TensorRT optimized output for: " + input_data;
}
