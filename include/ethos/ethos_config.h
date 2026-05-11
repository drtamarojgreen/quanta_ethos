#pragma once

#include <string>
#include <vector>

class EthicalFramework {
public:
    EthicalFramework(const std::string& name);
    void addRule(const std::string& rule);
    std::vector<std::string> getRules() const;
    std::string getName() const;

private:
    std::string name;
    std::vector<std::string> rules;
};

class EthosConfig {
public:
    EthosConfig();
    void setActiveFramework(const std::string& name);
    void registerFramework(const EthicalFramework& framework);
    EthicalFramework* getActiveFramework();

private:
    std::string active_framework;
    std::vector<EthicalFramework> frameworks;
};
