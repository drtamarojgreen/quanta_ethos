#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

struct Turn {
    std::string user_input;
    std::string assistant_output;
};

class Session {
public:
    Session(const std::string& id);
    void addTurn(const std::string& input, const std::string& output);
    const std::vector<Turn>& getHistory() const;
    std::string getId() const;

private:
    std::string id;
    std::vector<Turn> history;
};

class SessionManager {
public:
    SessionManager();
    std::shared_ptr<Session> getOrCreateSession(const std::string& id);
    void removeSession(const std::string& id);

private:
    std::map<std::string, std::shared_ptr<Session>> sessions;
};
