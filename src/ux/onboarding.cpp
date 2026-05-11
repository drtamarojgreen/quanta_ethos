#include "onboarding.h"

Onboarding::Onboarding() : completed(3, false) {}

std::vector<std::string> Onboarding::getTutorialSteps() {
    return {"Welcome to PrismQuanta", "How to use the CLI", "Understanding Trust Scores"};
}

void Onboarding::markStepComplete(int id) {
    if (id >= 0 && id < (int)completed.size()) completed[id] = true;
}
