#pragma once

#include "ux/terminal_view.h"
#include <iostream>

class CoreEngineView : public ITerminalView {
public:
    std::string getTitle() const override { return "Core Engine"; }
    bool isVisible(const ConfigEngine& config) const override {
        return config.get("view.core_engine.visible", "true") == "true";
    }
    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mCore Engine Settings\033[0m\n";
        std::cout << "\033[2;" << startCol << "H1. Model Path: " << config.get("engine.model_path", "prismquanta_v1.bin") << "\n";
        std::cout << "\033[3;" << startCol << "H2. Threads: " << config.getInt("engine.threads", 4) << "\n";
        std::cout << "\033[4;" << startCol << "H3. Context Size: " << config.getInt("engine.context_size", 2048) << "\n";
        std::cout << "\033[6;" << startCol << "H[1-3] Edit Value";
    }
    void handleInput(int input, ConfigEngine& config) override {
        if (input == '1') {
            config.set("engine.model_path", config.get("engine.model_path") == "prismquanta_v1.bin" ? "prismquanta_v2.bin" : "prismquanta_v1.bin");
        } else if (input == '2') {
            config.set("engine.threads", std::to_string((config.getInt("engine.threads") % 8) + 1));
        } else if (input == '3') {
            config.set("engine.context_size", std::to_string(config.getInt("engine.context_size") == 2048 ? 4096 : 2048));
        }
    }
};

class ProgressView : public ITerminalView {
public:
    std::string getTitle() const override { return "Progress Center"; }
    bool isVisible(const ConfigEngine& config) const override { return true; }
    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mBackground Tasks\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- Pipeline State: IDLE\n";
        std::cout << "\033[3;" << startCol << "H- Active Tasks:   0\n";
    }
    void handleInput(int input, ConfigEngine& config) override {}
};

class EthosView : public ITerminalView {
public:
    std::string getTitle() const override { return "Ethical Gov"; }
    bool isVisible(const ConfigEngine& config) const override {
        return config.get("view.ethos.visible", "true") == "true";
    }
    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mEthical Governance\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- Toxicity Filter: [" << config.get("ethos.toxicity_filter", "ENABLED") << "]\n";
        std::cout << "\033[3;" << startCol << "H- Bias Scanner: [" << config.get("ethos.bias_scanner", "ENABLED") << "]\n";
        std::cout << "\033[4;" << startCol << "H- PII Redactor: [" << config.get("ethos.pii_redactor", "DISABLED") << "]\n";
        std::cout << "\033[6;" << startCol << "H[T/B/P] Toggle Policies";
    }
    void handleInput(int input, ConfigEngine& config) override {
        if (input == 't' || input == 'T') {
            config.set("ethos.toxicity_filter", config.get("ethos.toxicity_filter") == "ENABLED" ? "DISABLED" : "ENABLED");
        } else if (input == 'b' || input == 'B') {
            config.set("ethos.bias_scanner", config.get("ethos.bias_scanner") == "ENABLED" ? "DISABLED" : "ENABLED");
        } else if (input == 'p' || input == 'P') {
            config.set("ethos.pii_redactor", config.get("ethos.pii_redactor") == "ENABLED" ? "DISABLED" : "ENABLED");
        }
    }
};

class ModelView : public ITerminalView {
public:
    std::string getTitle() const override { return "Model Backend"; }
    bool isVisible(const ConfigEngine& config) const override {
        return config.get("view.model_backend.visible", "true") == "true";
    }
    void render(const ConfigEngine& config, int startCol) const override {
        std::cout << "\033[1mModel Backend Config\033[0m\n";
        std::cout << "\033[2;" << startCol << "H- Backend: " << config.get("model.backend", "llama.cpp") << "\n";
        std::cout << "\033[3;" << startCol << "H- Quantization: " << config.get("model.quantization", "4-bit") << "\n";
        std::cout << "\033[4;" << startCol << "H- HW Accel: " << config.get("model.hw_accel", "CUDA") << "\n";
        std::cout << "\033[6;" << startCol << "H[B/Q/H] Switch Config";
    }
    void handleInput(int input, ConfigEngine& config) override {
        if (input == 'b' || input == 'B') {
            config.set("model.backend", config.get("model.backend") == "llama.cpp" ? "onnx" : "llama.cpp");
        } else if (input == 'q' || input == 'Q') {
            config.set("model.quantization", config.get("model.quantization") == "4-bit" ? "8-bit" : "4-bit");
        } else if (input == 'h' || input == 'H') {
            config.set("model.hw_accel", config.get("model.hw_accel") == "CUDA" ? "CPU" : "CUDA");
        }
    }
};
