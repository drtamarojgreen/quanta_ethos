#pragma once

#include <string>
#include <vector>

struct Feedback {
    std::string response_id;
    int rating; // 1-5
    std::string comment;
};

class FeedbackManager {
public:
    FeedbackManager();
    void submitFeedback(const Feedback& fb);
    std::vector<Feedback> getFeedbackForResponse(const std::string& id) const;
    float getAverageRating() const;

private:
    std::vector<Feedback> feedback_store;
};
