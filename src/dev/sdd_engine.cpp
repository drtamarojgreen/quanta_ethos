#include "dev/sdd_engine.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>
#include <sstream>

// Simple XML parser for our specific scoring rules schema
class SimpleXmlParser {
public:
    static std::map<std::string, std::string> parseTag(const std::string& xml, const std::string& tag) {
        std::map<std::string, std::string> result;
        std::regex tagRegex("<" + tag + ">(.*?)</" + tag + ">");
        std::smatch match;
        if (std::regex_search(xml, match, tagRegex)) {
            result["content"] = match[1].str();
        }
        return result;
    }

    static std::vector<std::map<std::string, std::string>> parseList(const std::string& xml, const std::string& parentTag, const std::string& childTag) {
        std::vector<std::map<std::string, std::string>> result;
        std::regex parentRegex("<" + parentTag + ">(.*?)</" + parentTag + ">", std::regex::single_line);
        std::smatch parentMatch;
        if (std::regex_search(xml, parentMatch, parentRegex)) {
            std::string content = parentMatch[1].str();
            std::regex childRegex("<" + childTag + "([^>]*)>(.*?)</" + childTag + ">", std::regex::single_line);
            auto begin = std::sregex_iterator(content.begin(), content.end(), childRegex);
            auto end = std::sregex_iterator();
            for (auto i = begin; i != end; ++i) {
                std::map<std::string, std::string> entry;
                entry["content"] = (*i)[2].str();
                std::string attrs = (*i)[1].str();
                std::regex attrRegex("(\\w+)=\"([^\"]*)\"");
                auto abegin = std::sregex_iterator(attrs.begin(), attrs.end(), attrRegex);
                for (auto j = abegin; j != end; ++j) {
                    entry[(*j)[1].str()] = (*j)[2].str();
                }
                result.push_back(entry);
            }
        }
        return result;
    }
};

std::pair<std::vector<SddFailure>, std::vector<std::string>> FactValidator::validate(const fs::path& sddRoot) {
    std::vector<SddFailure> failures;
    std::vector<std::string> keys;
    fs::path factsDir = sddRoot / "facts";

    if (!fs::exists(factsDir) || !fs::is_directory(factsDir)) {
        return {failures, keys};
    }

    int strictCount = 0;
    int totalFiles = 0;

    for (const auto& entry : fs::recursive_directory_iterator(factsDir)) {
        if (entry.path().extension() == ".facts") {
            totalFiles++;
            std::ifstream file(entry.path());
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            bool hasSit = content.find("Situation:") != std::string::npos;
            bool hasLevel = std::regex_search(content, std::regex("^(Is|Needs|Results)\\s+", std::regex::multiline));

            if (hasSit && hasLevel) {
                strictCount++;
            }

            std::regex keyRegex("^(Is|Needs|Results)\\s+([\\w.]+)\\s*=");
            auto words_begin = std::sregex_iterator(content.begin(), content.end(), keyRegex);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                keys.push_back((*i)[2].str());
            }
        }
    }

    if (totalFiles > 0 && strictCount != totalFiles) {
        failures.push_back({"Facts", "Not all fact files follow strict Situation/Level syntax.", "Update all .facts files to use Situation headers and Level prefixes."});
    }

    return {failures, keys};
}

SddEngine::SddEngine() {
    loadScoringRules();
}

fs::path SddEngine::findSddRoot(const fs::path& repoPath) {
    std::vector<std::string> searchPaths = {"test/sdd", "tests/sdd", "tests/sorrel/sdd", "sdd"};
    for (const auto& p : searchPaths) {
        fs::path full = repoPath / p;
        if (fs::exists(full) && fs::is_directory(full)) return full;
    }
    return "";
}

void SddEngine::loadScoringRules() {
    fs::path xmlPath = "data/sdd_scoring_rules.xml";
    if (!fs::exists(xmlPath)) {
        // Fallback defaults
        fileWeights_["sorrel_checkins.md"] = 5;
        fileWeights_["sorrel_checkouts.md"] = 5;
        fileWeights_["restrictions.md"] = 10;
        fileWeights_["card_runner.cpp"] = 5;
        bonusWeights_["sorrel_executable"] = 15;
        bonusWeights_["sip_commands"] = 10;
        bonusWeights_["better_fact_structure"] = 10;
        penalties_["placeholder_usage"] = -20;
        penalties_["bash_cli_replacement"] = -15;
        return;
    }

    std::ifstream file(xmlPath);
    std::string xml((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    auto structural = SimpleXmlParser::parseTag(xml, "StructuralWeight");
    if (structural.count("content")) structuralWeight_ = std::stoi(structural["content"]);

    auto files = SimpleXmlParser::parseList(xml, "FileWeights", "File");
    for (const auto& f : files) {
        if (f.count("name") && f.count("weight")) fileWeights_[f.at("name")] = std::stoi(f.at("weight"));
    }

    auto bonuses = SimpleXmlParser::parseList(xml, "BonusWeights", "Bonus");
    for (const auto& b : bonuses) {
        if (b.count("name") && b.count("weight")) bonusWeights_[b.at("name")] = std::stoi(b.at("weight"));
    }

    auto penalties = SimpleXmlParser::parseList(xml, "Penalties", "Penalty");
    for (const auto& p : penalties) {
        if (p.count("name") && p.count("value")) penalties_[p.at("name")] = std::stoi(p.at("value"));
    }

    auto cardWeights = SimpleXmlParser::parseTag(xml, "CardWeights");
    if (cardWeights.count("content")) {
        auto base = SimpleXmlParser::parseTag(cardWeights["content"], "BaseWeight");
        if (base.count("content")) cardBaseWeight_ = std::stoi(base["content"]);
        auto results = SimpleXmlParser::parseTag(cardWeights["content"], "ResultsDecoratorWeight");
        if (results.count("content")) cardResultsWeight_ = std::stoi(results["content"]);
    }
}

std::vector<CoverageEntry> RegexCoverageScanner::scan(const fs::path& repoPath) {
    std::vector<CoverageEntry> coverage;
    std::vector<std::string> extensions = {".cpp", ".h", ".py", ".js", ".go", ".java", ".rs"};

    // Pattern matches // @Card: <name> or # @Card: <name>
    std::regex cardRegex(R"((?:\/\/|#)\s*@Card:\s*([\w.-]+))");

    for (const auto& entry : fs::recursive_directory_iterator(repoPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
                std::ifstream file(entry.path());
                std::string line;
                CoverageEntry ce;
                ce.filePath = fs::relative(entry.path(), repoPath).string();

                while (std::getline(file, line)) {
                    std::smatch match;
                    if (std::regex_search(line, match, cardRegex)) {
                        ce.cards.push_back(match[1].str());
                        ce.isCovered = true;
                    }
                }
                coverage.push_back(ce);
            }
        }
    }
    return coverage;
}

SddReport SddEngine::runCheck(const std::string& repoPath) {
    SddReport report;
    fs::path root(repoPath);
    fs::path sddRoot = findSddRoot(root);

    if (sddRoot.empty()) {
        report.failures.push_back({"Structure", "SDD root directory not found.", "Create tests/sdd/ or sdd/ directory."});
    } else {
        for (const auto& [file, weight] : fileWeights_) {
            if (fs::exists(sddRoot / file)) {
                report.score += weight;
            } else {
                report.failures.push_back({"Files", file + " missing.", "Add " + file + " to the SDD root."});
            }
        }

        FactValidator validator;
        auto [factFails, keys] = validator.validate(sddRoot);
        report.failures.insert(report.failures.end(), factFails.begin(), factFails.end());
        if (factFails.empty() && !keys.empty()) {
            report.score += bonusWeights_["better_fact_structure"];
        }

        fs::path sorrelExe = sddRoot / "sorrel";
        if (fs::exists(sorrelExe)) {
            report.score += bonusWeights_["sorrel_executable"];
            // Check for SIP commands in the binary would be complex, simplified for now
            report.score += bonusWeights_["sip_commands"];
        } else {
            report.failures.push_back({"SORREL CLI", "sorrel executable missing in SDD root.", "Implement SORREL CLI as a compiled binary."});
        }

        scanCards(sddRoot, report);
    }

    scanPenalties(root, report);
    scanModularity(root, report);

    report.status = (report.score > 50) ? "Compliant" : "Needs Improvement";

    RegexCoverageScanner scanner;
    report.coverage = scanner.scan(root);

    return report;
}

std::vector<CoverageEntry> SddEngine::runCoverage(const std::string& repoPath) {
    RegexCoverageScanner scanner;
    return scanner.scan(fs::path(repoPath));
}

void SddEngine::scanCards(const fs::path& sddRoot, SddReport& report) {
    fs::path cardsDir = sddRoot / "cards";
    if (!fs::exists(cardsDir)) {
        report.failures.push_back({"Cards", "cards/ directory missing.", "Create cards/ directory and implement SDD card classes."});
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(cardsDir)) {
        if (entry.path().extension() == ".cpp") {
            std::ifstream file(entry.path());
            std::string line;
            bool hasResults = false;
            int cardCount = 0;
            while (std::getline(file, line)) {
                if (line.find("@Card:") != std::string::npos) cardCount++;
                if (line.find("@Results") != std::string::npos) hasResults = true;
            }
            report.score += cardCount * cardBaseWeight_;
            if (hasResults) report.score += cardCount * cardResultsWeight_;
        }
    }
}

void SddEngine::scanPenalties(const fs::path& repoPath, SddReport& report) {
    std::vector<std::string> badMarkers = {"placeholder", "stub", "todo"};
    std::vector<std::string> scanDirs = {"src", "test", "tests", "include"};

    bool foundMarker = false;
    for (const auto& sd : scanDirs) {
        fs::path fullPath = repoPath / sd;
        if (!fs::exists(fullPath)) continue;

        for (const auto& entry : fs::recursive_directory_iterator(fullPath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".cpp" || ext == ".h" || ext == ".py" || ext == ".md") {
                    if (entry.path().filename() == "sdd_checker.py" || entry.path().filename() == "sdd_engine.cpp") continue;

                    std::ifstream file(entry.path());
                    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    std::transform(content.begin(), content.end(), content.begin(), ::tolower);

                    for (const auto& m : badMarkers) {
                        if (content.find(m) != std::string::npos) {
                            report.failures.push_back({"Source Code", "Forbidden marker '" + m + "' found in " + entry.path().string(), "Remove all temporary markers and fully implement features."});
                            foundMarker = true;
                            break;
                        }
                    }
                }
            }
            if (foundMarker) break;
        }
        if (foundMarker) break;
    }

    if (foundMarker) report.score += penalties_["placeholder_usage"];
}

void SddEngine::scanModularity(const fs::path& repoPath, SddReport& report) {
    int interfaceCount = 0;
    int implementationCount = 0;
    int dirCount = 0;

    for (const auto& entry : fs::recursive_directory_iterator(repoPath)) {
        if (entry.is_directory()) {
            if (entry.path().filename().string().find('.') != 0 &&
                entry.path().filename() != "build" &&
                entry.path().filename() != "node_modules") {
                dirCount++;
            }
        } else if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".h" || ext == ".hpp") interfaceCount++;
            if (ext == ".cpp" || ext == ".cc" || ext == ".c") implementationCount++;
        }
    }

    // Modular if:
    // 1. Decent directory partitioning (more than 3 logical subdirs)
    // 2. High interface-to-implementation ratio (aiming for roughly 1:1 or better)
    bool goodPartitioning = (dirCount > 3);
    bool goodRatio = (implementationCount > 0) && ( (float)interfaceCount / implementationCount > 0.8f );

    if (goodPartitioning && goodRatio) {
        report.score += bonusWeights_["modularity"];
    } else {
        if (!goodPartitioning) {
            report.failures.push_back({"Architecture", "Monolithic structure detected.", "Partition code into logical subdirectories (e.g., logic, model, api)."});
        }
        if (!goodRatio) {
            report.failures.push_back({"Architecture", "Low interface-to-implementation ratio.", "Define more abstract interfaces (headers) for your components."});
        }
    }
}

void SddEngine::initSdd(const std::string& repoPath) {
    fs::path root(repoPath);
    fs::path sddRoot = root / "test/sdd";
    fs::create_directories(sddRoot / "cards");
    fs::create_directories(sddRoot / "facts");

    auto write_file = [](const fs::path& p, const std::string& c) {
        if (!fs::exists(p)) {
            std::ofstream f(p);
            f << c;
        }
    };

    write_file(sddRoot / "sorrel_checkins.md", "# SDD Checkins\n");
    write_file(sddRoot / "sorrel_checkouts.md", "# SDD Checkouts\n");
    write_file(sddRoot / "restrictions.md", "# SDD Restrictions\n");
}
