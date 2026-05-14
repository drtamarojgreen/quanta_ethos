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

std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) start++;
    auto end = s.end();
    do { end--; } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

std::string execute_command(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) return "ERROR: popen failed";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();
    return result;
}

int main(int argc, char* argv[]) {
    fs::path sdd_dir = fs::current_path(); // Assume running from test/sdd
    fs::path facts_path = sdd_dir / "facts" / "env_facts.json";

    std::cout << "SORREL Card Runner" << std::endl;
    std::cout << "Discovery root: " << sdd_dir << std::endl;

    fs::path cards_dir = sdd_dir / "cards";
    if (!fs::exists(cards_dir)) {
        std::cerr << "Error: cards directory not found." << std::endl;
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(cards_dir)) {
        if (entry.path().extension() == ".cpp") {
            std::cout << "Processing Class: " << entry.path().filename() << std::endl;
            // In a real runner, we'd compile and execute.
            // For this task, we are providing the structural component.
        }
    }

    return 0;
}
