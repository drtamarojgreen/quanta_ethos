#include "async_io.h"
#include <thread>
#include <fstream>
#include <sstream>

AsyncIo::AsyncIo() {}

std::future<std::string> AsyncIo::readFileAsync(const std::string& path) {
    return std::async(std::launch::async, [path]() {
        std::ifstream f(path);
        std::stringstream ss;
        ss << f.rdbuf();
        return ss.str();
    });
}
