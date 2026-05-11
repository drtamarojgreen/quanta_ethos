#include "pii_redactor.h"
#include <regex>

PiiRedactor::PiiRedactor() {}

std::string PiiRedactor::redact(const std::string& text) {
    std::string result = text;
    // Redact email addresses
    std::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    result = std::regex_replace(result, email_regex, "[EMAIL_REDACTED]");

    // Redact phone numbers (simple pattern)
    std::regex phone_regex(R"(\d{3}-\d{3}-\d{4})");
    result = std::regex_replace(result, phone_regex, "[PHONE_REDACTED]");

    return result;
}
