#pragma once

#include <string>
#include <future>

class AsyncIo {
public:
    AsyncIo();
    std::future<std::string> readFileAsync(const std::string& path);
};
