# CHAI Restrictions

## Pattern Restrictions
- Prohibit empty catch blocks.
- Prohibit meaningless assertions (e.g., asserting string is not empty).
- All assertions must verify specific expected empirical values.

## Tool Restrictions
- Allowed tools: g++, cmake, ctest.
- Allowed libraries: dlfcn.h, std::thread, std::future, and standard C++17 headers.

## Architectural Restrictions
- Mandatory use of interfaces for all 100 features.
- Components must be independently testable.
- No direct implementation in CoreEngine; use injected components.

## Validation Restrictions
- All validation logic must reference real outputs.
- 100% test coverage for all new public interfaces.
- UI Layout Verification: Content start column must be calculated as `(Width / 4) + 2`.
- Input Verification: Terminal must be restored to `oldt` state before exit.

## Sorrel C++ Checker Restrictions
- `SddEngine` must be independent of any UI framework.
- Coverage scanner must support: .cpp, .h, .py, .js, .go, .java, .rs.
- `ScoringEngine` must load weights and penalties from `data/sdd_scoring_rules.xml`.
- All TUI components must use ANSI escape codes only (no ncurses).
- The `sorrel` CLI must be a standalone compiled C++ binary.
- `SddEngine` must use the SIP principle: minimal execution increments.
