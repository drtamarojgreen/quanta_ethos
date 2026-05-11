#pragma once

#include <string>

class EtlWorker {
public:
    EtlWorker(const std::string& warehouse_url);
    bool extract(const std::string& source_path);
    bool transform();
    bool load();

private:
    std::string url;
};
