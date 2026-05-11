#include "context_help.h"

ContextHelp::ContextHelp() {
    help_db["main"] = "Main menu help...";
    help_db["generate"] = "Generation help...";
}

std::string ContextHelp::getHelp(const std::string& id) {
    auto it = help_db.find(id);
    return it != help_db.end() ? it->second : "No help available for this context.";
}
