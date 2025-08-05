#include "model_backend.h"

ModelBackend::ModelBackend() {}

std::string ModelBackend::run_model(const std::string& input) {
    return "Model output for: " + input;
}
