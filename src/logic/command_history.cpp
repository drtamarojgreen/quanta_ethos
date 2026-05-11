#include "command_history.h"

CommandManager::CommandManager() {}

void CommandManager::executeCommand(std::unique_ptr<ICommand> command) {
    command->execute();
    history.push_back(std::move(command));
    redo_stack.clear();
}

void CommandManager::undo() {
    if (history.empty()) return;
    auto command = std::move(history.back());
    history.pop_back();
    command->undo();
    redo_stack.push_back(std::move(command));
}

void CommandManager::redo() {
    if (redo_stack.empty()) return;
    auto command = std::move(redo_stack.back());
    redo_stack.pop_back();
    command->execute();
    history.push_back(std::move(command));
}
