#include "plugin_loader.h"
#include <dlfcn.h>
#include <iostream>

PluginLoader::PluginLoader() {}

std::unique_ptr<IPlugin> PluginLoader::loadPlugin(const std::string& path) {
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return nullptr;
    }

    typedef IPlugin* (*create_t)();
    create_t create_plugin = (create_t)dlsym(handle, "create_plugin");

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol 'create_plugin': " << dlsym_error << '\n';
        dlclose(handle);
        return nullptr;
    }

    return std::unique_ptr<IPlugin>(create_plugin());
}
