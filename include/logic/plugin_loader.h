#pragma once

#include <string>
#include <memory>
#include <vector>

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string getName() const = 0;
    virtual void initialize() = 0;
};

class PluginLoader {
public:
    PluginLoader();
    std::unique_ptr<IPlugin> loadPlugin(const std::string& path);
};
