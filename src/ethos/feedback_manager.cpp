#include "feedback_manager.h"
#include <numeric>

FeedbackManager::FeedbackManager() {}

void FeedbackManager::submitFeedback(const Feedback& fb) {
    feedback_store.push_back(fb);
}

std::vector<Feedback> FeedbackManager::getFeedbackForResponse(const std::string& id) const {
    std::vector<Feedback> results;
    for (const auto& fb : feedback_store) {
        if (fb.response_id == id) results.push_back(fb);
    }
    return results;
}

float FeedbackManager::getAverageRating() const {
    if (feedback_store.empty()) return 0.0f;
    float sum = std::accumulate(feedback_store.begin(), feedback_store.end(), 0.0f,
        [](float a, const Feedback& b) { return a + b.rating; });
    return sum / feedback_store.size();
}
