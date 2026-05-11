#include "source_attributor.h"
#include <sstream>

SourceAttributor::SourceAttributor() {}

void SourceAttributor::addAttribution(const std::string& statement, const std::string& source) {
    attributions.push_back({statement, source});
}

std::string SourceAttributor::appendSources(const std::string& output) const {
    if (attributions.empty()) return output;

    std::ostringstream oss;
    oss << output << "\n\nSources:\n";
    for (const auto& attr : attributions) {
        oss << "- " << attr.statement << " [" << attr.source << "]\n";
    }
    return oss.str();
}
