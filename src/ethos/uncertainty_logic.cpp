#include "uncertainty_logic.h"

UncertaintyLogic::UncertaintyLogic(float threshold) : threshold(threshold) {}

std::string UncertaintyLogic::processResponse(const std::string& response, float confidence) {
    if (confidence < threshold) {
        return "I am sorry, but I do not have enough certain information to answer that accurately.";
    }
    return response;
}
