# PrismQuanta: The Principled AI Ecosystem

**PrismQuanta** is a framework for building a cohesive, autonomous, and ethically-governed AI ecosystem from a collection of specialized modules. The system is designed to be local-first, modular, resilient, and to operate under a strict ethical framework.

This project is guided by the **PrismQuanta philosophy**, which prioritizes integrity, safety, and transparency in artificial intelligence.

## Vision and Guiding Principles

The goal of this project is to unify the discrete PrismQuanta modules into a single, cohesive, and autonomous system. The integration will adhere to the project's core philosophies:

-   **Local-First & Offline:** The integrated system must operate without external network dependencies, relying on local resources like `llama.cpp`.
-   **Modularity & Loose Coupling:** Components must be independently testable and replaceable. Communication should occur over well-defined, simple interfaces.
-   **Minimalism & Resilience:** Prefer simple, robust technologies (e.g., shell scripts, file-based queues, standard CLI tools) over complex frameworks to ensure stability in constrained environments.
-   **Ethical Governance:** `QuantaEthos` must be a foundational, non-bypassable layer for all significant actions undertaken by the system.

## High-Level Architecture

The system is organized into layers of responsibility, from high-level strategy to low-level execution. Key components include:

-   **QuantaLista (Strategist):** Manages the master project plan.
-   **QuantaPorto (Commander):** The tactical controller that translates tasks into executable actions.
-   **QuantaSensa (Field Agent):** The hands of the system that executes actions.
-   **QuantaGlia (Librarian):** The knowledge manager responsible for self-improvement.
-   **QuantaEthos (Conscience):** A mandatory ethical validation service.

For a detailed explanation of the architecture and data flow, please see [`docs/architecture.md`](docs/architecture.md).

## Getting Started

PrismQuanta is designed to be platform-independent. The recommended build tool is **CMake**.

### 1. Prerequisites

*   **Compiler:** A C++17 compliant compiler (e.g., `g++` 9+, `clang` 10+, or MSVC 2019+).
*   **Build System:** `CMake` 3.16 or later.
*   **Tools (Optional):** `Ninja` or `Make`.

### 2. Build Instructions (Cross-Platform)

The following commands work in any terminal (Bash, PowerShell, Command Prompt):

```bash
# 1. Create and enter a build directory
cmake -B build

# 2. Setup the environment (directories and .env)
cmake --build build --target setup

# 3. Build the entire ecosystem
cmake --build build

# 4. Run tests
cmake --build build --target test_integration
```

### 3. Cleaning the Build

To remove build artifacts and start fresh:

#### Using CMake
```bash
cmake --build build --target clean
```

#### Manual Cleanup
*   **Linux/macOS:** `rm -rf build/*`
*   **Windows (PowerShell):** `Remove-Item -Recurse -Force build/*`

### 3. Native g++ Instructions

If you prefer to use `g++` directly (e.g., if CMake is not available), you can compile the tools manually from the root directory:

#### Standalone Ethical Validator (`quanta_ethos`)
*   **Linux:** `g++ -std=c++17 -Iinclude src/QuantaEthos/main.cpp src/QuantaEthos/ethics_logic.cpp -o quanta_ethos`
*   **Windows:** `g++ -std=c++17 -Iinclude src/QuantaEthos/main.cpp src/QuantaEthos/ethics_logic.cpp -o quanta_ethos.exe`

#### Main Application with UI (`prismquanta`)
This requires compiling the core library and the UI components together.
*   **Linux:**
    ```bash
    g++ -std=c++17 -Iinclude -Isrc/QuantaEthos \
        src/main.cpp src/core_engine.cpp \
        src/logic/*.cpp src/ux/*.cpp src/dev/sdd_engine.cpp \
        src/QuantaEthos/ethics_logic.cpp \
        -o prismquanta
    ```
*   **Windows:**
    ```powershell
    g++ -std=c++17 -Iinclude -Isrc/QuantaEthos `
        src/main.cpp src/core_engine.cpp `
        src/logic/*.cpp src/ux/*.cpp src/dev/sdd_engine.cpp `
        src/QuantaEthos/ethics_logic.cpp `
        -o prismquanta.exe
    ```

## Troubleshooting & Alternatives

### Missing CMake
If `cmake` is missing, use the **Native g++ Instructions** above. We recommend installing CMake from [cmake.org](https://cmake.org/download/) for easier management of the full ecosystem.

### Missing g++ / MinGW (Windows)
If you don't have a compiler, we recommend installing [MSYS2](https://www.msys2.org/) and running `pacman -S mingw-w64-x86_64-toolchain` to get a complete C++ environment.

### "src\prismquanta.exe" Error in PowerShell
If you see an error about a "module 'src' could not be loaded", it is because PowerShell is misinterpreting the path. Always run local executables with the `.\` prefix:
`.\build\src\prismquanta.exe`

### Legacy Scripts
The `.sh` scripts (`build_all.sh`, `test_all.sh`, `start_system.sh`) are now deprecated in favor of the CMake-driven workflow. They are maintained for POSIX-compliant environments but may not work natively on Windows.

## Sorrel Driven Development (SDD) Tools

PrismQuanta includes a suite of C++ tools for enforcing SDD principles.

### Sorrel CLI (`sorrel`)

The `sorrel` binary is the primary interface for SDD adherence checking and project initialization. Build it first using `cmake --build build --target sorrel`.

#### Commands (Windows)
*   `.\build\sorrel.exe check .` : Evaluates the repository against SDD rules.
*   `.\build\sorrel.exe init <path>` : Bootstraps a minimal SDD structure.
*   `.\build\sorrel.exe coverage .` : Scans for `@Card` coverage in source files.
*   `.\build\sorrel.exe sip` : Executes a "Small Incremental Progress" step.

#### Commands (Linux)
*   `./build/sorrel check .`
*   `./build/sorrel init <path>`
*   `./build/sorrel coverage .`
*   `./build/sorrel sip`

### Installation

Any repository can install the Sorrel tools by running:

```bash
curl -sSL {this-repository}/install/sorrel_checker.sh | bash
```

### SDD Scoring and Modularity

The checker uses a rules-based scoring engine (`data/sdd_scoring_rules.xml`). It specifically rewards:
-   **Strict Fact Syntax**: Use of `Situation:`, `Is`, `Needs`, `Results`.
-   **Card Coverage**: Implementation of executable cards for all features.
-   **Modularity**: Logical partitioning into subdirectories and high interface-to-implementation ratios.

## The PrismQuanta Ethical Framework

This project is not just about technology; it's about building AI that is principled, transparent, and aligned with human values. The ethical framework is a core part of the project and is detailed in the following documents:

*   `docs/vision.md`: The guiding philosophy of the project.
*   `docs/ethical_checkpoints.md`: The decision-making process for the AI agent.
*   `docs/integration_policy.md`: Guidelines for responsibly integrating LLMs into a workplace.
*   `docs/behavior_profile.md`: A guide to understanding the LLM's behavior.
*   `docs/disclaimer.md`: A disclaimer written from the perspective of the LLM.
*   `docs/safety_controls.md`: Details on the Human-in-the-Loop (HITL) and safety controls.

We encourage you to read these documents to understand the project's commitment to ethical AI.

## Contributing

We welcome contributions to the project. Please follow these guidelines:

*   **Branching:** Create a new feature branch for your changes (e.g., `feature/my-new-feature`).
*   **Coding Style:** Adhere to the conventions outlined in the `docs/coding_style.md` document.
*   **Pull Requests:** Submit a pull request to the `main` branch. Provide a clear description of your changes and reference any related issues.

## License

This project is licensed under the MIT License.
