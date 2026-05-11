#pragma once

#include <string>
#include <vector>

struct Attribution {
    std::string statement;
    std::string source;
};

class SourceAttributor {
public:
    SourceAttributor();
    void addAttribution(const std::string& statement, const std::string& source);
    std::string appendSources(const std::string& output) const;

private:
    std::vector<Attribution> attributions;
};
