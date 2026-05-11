#pragma once

#include <string>
#include <map>

class SelfCorrection {
public:
    SelfCorrection();
    void recordOriginal(const std::string& interaction_id, const std::string& output);
    void submitCorrection(const std::string& interaction_id, const std::string& corrected_output);
    std::string getLatestVersion(const std::string& interaction_id) const;
    bool wasCorrected(const std::string& interaction_id) const;

private:
    std::map<std::string, std::string> originals;
    std::map<std::string, std::string> corrections;
};
