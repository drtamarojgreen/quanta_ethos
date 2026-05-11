#include "logic/session_manager.h"
#include "logic/command_history.h"
#include "test_framework.h"

TEST_CASE(bdd_multi_turn_conversation_flow) {
    // GIVEN a session manager
    SessionManager sm;
    auto session = sm.getOrCreateSession("user123");

    // WHEN a turn is added
    session->addTurn("Hello", "Hi there");

    // THEN the history should contain the turn
    ASSERT_EQ(1, session->getHistory().size());
    ASSERT_EQ("Hello", session->getHistory()[0].user_input);
}

TEST_CASE(bdd_undo_redo_interaction) {
    // GIVEN a command manager
    CommandManager cm;
    // ... commands ...
    cm.undo();
    // Verification of state after undo
}
