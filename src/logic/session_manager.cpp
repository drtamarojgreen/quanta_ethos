#include "session_manager.h"

Session::Session(const std::string& id) : id(id) {}

void Session::addTurn(const std::string& input, const std::string& output) {
    history.push_back({input, output});
}

const std::vector<Turn>& Session::getHistory() const {
    return history;
}

std::string Session::getId() const {
    return id;
}

SessionManager::SessionManager() {}

std::shared_ptr<Session> SessionManager::getOrCreateSession(const std::string& id) {
    if (sessions.find(id) == sessions.end()) {
        sessions[id] = std::make_shared<Session>(id);
    }
    return sessions[id];
}

void SessionManager::removeSession(const std::string& id) {
    sessions.erase(id);
}
