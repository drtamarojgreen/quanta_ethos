#include "model_backend.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include <algorithm>
#include "logic/config_engine.h"

ModelBackend::ModelBackend() {}

std::string ModelBackend::run_model(const std::string& input) {
    ConfigEngine config;
    config.load("prismquanta.conf");
    config.load(".quanta");
    
    std::string llama_path = config.get("model.llama_cli_path", "./llama-cli");
    std::string model_path = config.get("engine.model_path", "model.gguf");
    
    // Construct command with flags to minimize noise and ensure it exits
    std::stringstream cmd;
    cmd << llama_path << " -m " << model_path << " -p \"" << input << "\" -n 128 --temp 0 --no-display-prompt --log-disable --simple-io --single-turn --no-show-timings 2>/dev/null";
    
    std::array<char, 256> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.str().c_str(), "r"), pclose);
    
    if (!pipe) {
        return "Error: Failed to launch llama-cli.";
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    if (result.empty()) {
        return "Error: No output from model (is the model path correct?).";
    }

    // Surgical cleanup of llama-cli noise
    // The new llama-cli in "chat" mode prints a banner and then "> [Prompt]"
    // We want to extract just the response that follows.
    
    std::string prompt_marker = "> " + input;
    size_t start_pos = result.find(prompt_marker);
    if (start_pos != std::string::npos) {
        result = result.substr(start_pos + prompt_marker.length());
    } else {
        // Fallback: search for the last occurrence of the prompt if it was slightly modified
        // (e.g. by escaping or chat templates)
    }
    
    // Remove the "Exiting..." footer
    size_t exit_pos = result.find("Exiting...");
    if (exit_pos != std::string::npos) {
        result = result.substr(0, exit_pos);
    }

    // Trim leading/trailing whitespace and newlines
    auto trim = [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    };
    trim(result);
    
    if (result.empty()) {
        return "Error: Model returned an empty response after cleanup.";
    }
    
    return result;
}
