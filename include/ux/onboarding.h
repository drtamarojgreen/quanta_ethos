#pragma once

#include <string>
#include <vector>

class Onboarding {
public:
    Onboarding();
    std::vector<std::string> getTutorialSteps();
    void markStepComplete(int step_id);

private:
    std::vector<bool> completed;
};
