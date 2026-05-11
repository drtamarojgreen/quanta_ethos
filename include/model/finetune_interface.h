#pragma once

#include <string>

class FineTuneInterface {
public:
    FineTuneInterface();
    void prepareDataset(const std::string& interaction_logs_path, const std::string& output_dataset_path);
    bool startFineTuning(const std::string& config_path);
};
