#pragma once

#include <string>

class TensorRTWrapper {
public:
    TensorRTWrapper();
    bool optimizeModel(const std::string& model_path, const std::string& output_engine_path);
    std::string runInference(const std::string& input_data);
};
