#pragma once

#include <string>
#include <vector>
#include <memory>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class CommandManager {
public:
    CommandManager();
    void executeCommand(std::unique_ptr<ICommand> command);
    void undo();
    void redo();

private:
    std::vector<std::unique_ptr<ICommand>> history;
    std::vector<std::unique_ptr<ICommand>> redo_stack;
};
