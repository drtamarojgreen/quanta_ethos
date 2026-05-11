#pragma once

#include <string>
#include <memory>
#include <map>
#include "plugin_interfaces.h"

class ModelFactory {
public:
    ModelFactory();
    void registerBackend(const std::string& name, std::unique_ptr<IModelBackend> backend);
    IModelBackend* getBackend(const std::string& name);

private:
    std::map<std::string, std::unique_ptr<IModelBackend>> backends;
};
