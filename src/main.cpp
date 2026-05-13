#include <iostream>
#include <string>
#include <memory>

#include "core_engine.h"
#include "ux/terminal_ui.h"
#include "ux/subsystem_views.h"
#include "logic/config_engine.h"

int main(int argc, char* argv[]) {
    ConfigEngine config;
    config.load("prismquanta.conf");

    // Check for CLI flag
    if (argc > 1 && std::string(argv[1]) == "--cli") {
        std::string prompt = "Hello from PrismQuanta";
        if (argc > 2) {
            prompt = argv[2];
        }
        CoreEngine core_engine;
        std::cout << core_engine.generate_response(prompt) << std::endl;
        return 0;
    }

    // Launch Terminal UI by default
    TerminalUI ui(config);
    ui.addView(std::make_unique<CoreEngineView>());
    ui.addView(std::make_unique<EthosView>());
    ui.addView(std::make_unique<ModelView>());

    ui.run();

    return 0;
}
