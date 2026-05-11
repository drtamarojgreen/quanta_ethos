#include "ethos_config.h"

EthicalFramework::EthicalFramework(const std::string& name) : name(name) {}

void EthicalFramework::addRule(const std::string& rule) {
    rules.push_back(rule);
}

std::vector<std::string> EthicalFramework::getRules() const {
    return rules;
}

std::string EthicalFramework::getName() const {
    return name;
}

EthosConfig::EthosConfig() : active_framework("default") {
    EthicalFramework def("default");
    def.addRule("Do no harm.");
    frameworks.push_back(def);
}

void EthosConfig::setActiveFramework(const std::string& name) {
    active_framework = name;
}

void EthosConfig::registerFramework(const EthicalFramework& framework) {
    frameworks.push_back(framework);
}

EthicalFramework* EthosConfig::getActiveFramework() {
    for (auto& f : frameworks) {
        if (f.getName() == active_framework) return &f;
    }
    return nullptr;
}
