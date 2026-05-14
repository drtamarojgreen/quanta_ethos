#pragma once

#include "ux/terminal_view.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <array>
#include <cctype>
#include <future>
#include <algorithm>

class SddCheckerView : public ITerminalView {
public:
    std::string getTitle() const override { return "Sorrel Checker"; }

    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mSORREL Adherence Checker\033[0m\n";
        std::cout << "\033[2;" << startCol << "HRepo URL: " << repoUrl_ << "\n";
        std::cout << "\033[4;" << startCol << "H------------------------------\n";

        if (checking_) {
            if (checkFuture_.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                output_ = checkFuture_.get();
                checking_ = false;
            } else {
                std::cout << "\033[5;" << startCol << "HChecking in progress... " << spinner_[spinnerIndex_++ % 4] << "\n";
            }
        }

        int line = 5;
        for (const auto& outLine : output_) {
            std::string displayLine = outLine;
            if (displayLine.length() > 55) displayLine = displayLine.substr(0, 52) + "...";
            std::cout << "\033[" << line++ << ";" << startCol << "H" << displayLine << "\n";
            if (line > 18) break;
        }

        std::cout << "\033[20;" << startCol << "H[Enter] Run Check | [Backspace] Clear";
    }

    void handleInput(int input, ConfigEngine& config) override {
        if (checking_) return;

        if (input == 10 || input == 13) {
            if (!repoUrl_.empty()) startCheck();
        } else if (input == 127 || input == 8) {
            if (!repoUrl_.empty()) repoUrl_.pop_back();
        } else if (std::isprint(input)) {
            repoUrl_ += (char)input;
        }
    }

private:
    void startCheck() {
        checking_ = true;
        output_.clear();

        std::string sanitizedUrl = repoUrl_;
        sanitizedUrl.erase(std::remove_if(sanitizedUrl.begin(), sanitizedUrl.end(), [](char c) {
            return !std::isalnum(c) && c != '.' && c != '/' && c != ':' && c != '-' && c != '_';
        }), sanitizedUrl.end());

        checkFuture_ = std::async(std::launch::async, [sanitizedUrl]() {
            std::vector<std::string> results;
            std::string command = "./sorrel check " + sanitizedUrl + " 2>&1";
            std::array<char, 128> buffer;
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

            if (!pipe) {
                results.push_back("Error: Failed to run sorrel binary.");
                return results;
            }

            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                std::string line(buffer.data());
                if (!line.empty() && line.back() == '\n') line.pop_back();
                results.push_back(line);
            }
            return results;
        });
    }

    std::string repoUrl_ = "https://github.com/drtamarojgreen/quanta_glia";
    mutable std::vector<std::string> output_;
    mutable bool checking_ = false;
    mutable std::future<std::vector<std::string>> checkFuture_;
    mutable int spinnerIndex_ = 0;
    const std::vector<char> spinner_ = {'|', '/', '-', '\\'};
};
