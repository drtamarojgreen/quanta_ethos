#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

struct SddFailure {
    std::string component;
    std::string reason;
    std::string improvement;
};

struct CoverageEntry {
    std::string filePath;
    std::vector<std::string> cards;
    bool isCovered = false;
};

struct SddReport {
    int score = 0;
    std::string status;
    std::vector<SddFailure> failures;
    std::vector<CoverageEntry> coverage;
};

class RegexCoverageScanner {
public:
    std::vector<CoverageEntry> scan(const fs::path& repoPath);
};

class FactValidator {
public:
    std::pair<std::vector<SddFailure>, std::vector<std::string>> validate(const fs::path& sddRoot);
};

class SddEngine {
public:
    SddEngine();
    SddReport runCheck(const std::string& repoPath);
    std::vector<CoverageEntry> runCoverage(const std::string& repoPath);
    void initSdd(const std::string& repoPath);

private:
    fs::path findSddRoot(const fs::path& repoPath);
    void scanPenalties(const fs::path& repoPath, SddReport& report);
    void scanCards(const fs::path& sddRoot, SddReport& report);
    void scanModularity(const fs::path& repoPath, SddReport& report);

    std::map<std::string, int> fileWeights_;
    std::map<std::string, int> bonusWeights_;
    std::map<std::string, int> penalties_;
    int structuralWeight_ = 10;
    int cardBaseWeight_ = 2;
    int cardResultsWeight_ = 3;

    void loadScoringRules();
};
