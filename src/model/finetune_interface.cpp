#include "finetune_interface.h"
#include <iostream>

FineTuneInterface::FineTuneInterface() {}

void FineTuneInterface::prepareDataset(const std::string& logs, const std::string& output) {
    std::cout << "Preparing dataset from " << logs << ". Saving to " << output << "...\n";
}

bool FineTuneInterface::startFineTuning(const std::string& config) {
    std::cout << "Starting fine-tuning process with config: " << config << "...\n";
    return true;
}
