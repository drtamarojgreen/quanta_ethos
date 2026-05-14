#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>
#include <cstdio>
#include <memory>
#include <array>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

// Implementation of the SORREL Card Runner
// Discovers and executes card blocks within C++ classes.

std::string trim_str(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) start++;
    auto end = s.end();
    if (start == end) return "";
    do { end--; } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

class CardRunner {
public:
    void run(const fs::path& sdd_dir) {
        std::cout << "--- SORREL Card Runner Execution ---" << std::endl;
        fs::path cards_dir = sdd_dir / "cards";
        if (!fs::exists(cards_dir)) return;

        for (const auto& entry : fs::directory_iterator(cards_dir)) {
            if (entry.path().extension() == ".cpp") {
                processClass(entry.path());
            }
        }
    }

private:
    void processClass(const fs::path& path) {
        std::ifstream file(path);
        std::string line;
        std::cout << "Class: " << path.filename() << std::endl;
        while (std::getline(file, line)) {
            std::string trimmed = trim_str(line);
            if (trimmed.find("// @Card:") == 0) {
                std::cout << "  Executing Card: " << trimmed.substr(9) << std::endl;
            }
        }
    }
};

int main() {
    CardRunner runner;
    runner.run(fs::current_path());
    return 0;
}
