#include "model_factory.h"

ModelFactory::ModelFactory() {}

void ModelFactory::registerBackend(const std::string& name, std::unique_ptr<IModelBackend> backend) {
    backends[name] = std::move(backend);
}

IModelBackend* ModelFactory::getBackend(const std::string& name) {
    auto it = backends.find(name);
    if (it != backends.end()) {
        return it->second.get();
    }
    return nullptr;
}
