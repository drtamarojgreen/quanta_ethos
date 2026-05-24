# Precision Power SDD Restrictions

## Pattern Restrictions
- Prohibit empty catch blocks.
- Prohibit meaningless assertions (e.g., asserting string is not empty).
- All assertions must verify specific expected empirical values.
- No forbidden markers (T.O.D.O, S.T.U.B, p.l.a.c.e.h.o.l.d.e.r) in source code.

## Tool Restrictions
- Allowed tools: g++, cmake, ctest, python3.
- Allowed libraries: dlfcn.h, std::thread, std::future, and standard C++17 headers.
- Any external tool usage must be discovered and verified before use.

## Architectural Restrictions
- Mandatory use of interfaces for all features.
- Components must be independently testable.
- Sip Principle: Work must be performed in minimal increments (compile, run, observe, stop).
- Dual-Ledger: All work must be tracked in `sorrel_checkins.md` and `sorrel_checkouts.md`.
- No direct implementation in CoreEngine; use injected components.

## Validation Restrictions
- All validation logic must reference real outputs.
- Numeric Evidence Doctrine: All cards must produce numeric, machine-parseable observations.
- Self-certification using only boolean (pass/fail) is prohibited.
- Green Syntax: Cards and structured communications must use TOOLS, PARAMETERS, and RESULTS blocks.

## Sorrel C++ Checker Restrictions
- `SddEngine` must be independent of any UI framework.
- Coverage scanner must support: .cpp, .h, .py, .js, .go, .java, .rs.
- `ScoringEngine` must load weights and penalties from `data/sdd_scoring_rules.xml`.
- All TUI components must use ANSI escape codes only (no ncurses).
- The `sorrel` CLI must be a standalone compiled C++ binary.
- Reasoning Gate System (RGS): Plans must be validated against constraints before execution.
