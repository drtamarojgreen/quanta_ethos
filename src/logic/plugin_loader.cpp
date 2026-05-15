#include "plugin_loader.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

PluginLoader::PluginLoader() {}

std::unique_ptr<IPlugin> PluginLoader::loadPlugin(const std::string& path) {
#ifdef _WIN32
    HMODULE handle = LoadLibraryA(path.c_str());
    if (!handle) {
        DWORD error = GetLastError();
        std::cerr << "Cannot open library: Error code " << error << '\n';
        return nullptr;
    }

    typedef IPlugin* (*create_t)();
    create_t create_plugin = (create_t)GetProcAddress(handle, "create_plugin");

    if (!create_plugin) {
        std::cerr << "Cannot load symbol 'create_plugin'\n";
        FreeLibrary(handle);
        return nullptr;
    }
#else
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
#endif

    return std::unique_ptr<IPlugin>(create_plugin());
}
