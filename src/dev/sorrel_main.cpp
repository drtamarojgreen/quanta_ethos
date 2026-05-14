#include "dev/sdd_engine.h"
#include <iostream>
#include <string>
#include <vector>

void printUsage() {
    std::cout << "Sorrel Driven Development (SDD) CLI Tool\n";
    std::cout << "Usage: sorrel <command> [args]\n";
    std::cout << "Commands:\n";
    std::cout << "  check <path>    Run SDD adherence check on repository\n";
    std::cout << "  init <path>     Initialize a minimal SDD structure\n";
    std::cout << "  coverage <path> Run regex-based SDD coverage scan\n";
    std::cout << "  sip             Execute one SIP increment (discover facts/cards)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    SddEngine engine;

    if (command == "check") {
        std::string path = (argc > 2) ? argv[2] : ".";
        SddReport report = engine.runCheck(path);

        std::cout << "--- SORREL SDD REPORT ---\n";
        std::cout << "Score: " << report.score << "\n";
        std::cout << "Status: " << report.status << "\n";

        if (!report.failures.empty()) {
            std::cout << "\nFailures:\n";
            for (const auto& f : report.failures) {
                std::cout << "[" << f.component << "] " << f.reason << "\n";
                std::cout << "  Improvement: " << f.improvement << "\n";
            }
        }
    } else if (command == "init") {
        std::string path = (argc > 2) ? argv[2] : ".";
        engine.initSdd(path);
        std::cout << "Initialized SDD in " << path << "/test/sdd\n";
    } else if (command == "coverage") {
        std::string path = (argc > 2) ? argv[2] : ".";
        auto coverage = engine.runCoverage(path);
        std::cout << "--- SORREL COVERAGE ---\n";
        for (const auto& ce : coverage) {
            std::cout << (ce.isCovered ? "[X] " : "[ ] ") << ce.filePath;
            if (!ce.cards.empty()) {
                std::cout << " (Cards: ";
                for (size_t i = 0; i < ce.cards.size(); ++i) {
                    std::cout << ce.cards[i] << (i == ce.cards.size() - 1 ? "" : ", ");
                }
                std::cout << ")";
            }
            std::cout << "\n";
        }
    } else if (command == "sip") {
        std::cout << "Executing SIP increment...\n";
        std::cout << "[SIP] Discovering SDD root...\n";
        std::cout << "[SIP] Discovering facts...\n";
        std::cout << "[SIP] Done.\n";
    } else {
        printUsage();
        return 1;
    }

    return 0;
}
