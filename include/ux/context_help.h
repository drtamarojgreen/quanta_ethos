#pragma once

#include <string>
#include <map>

class ContextHelp {
public:
    ContextHelp();
    std::string getHelp(const std::string& context_id);

private:
    std::map<std::string, std::string> help_db;
};
