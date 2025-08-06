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

### Prerequisites

*   **Operating System:** Linux (Ubuntu 20.04 or later recommended).
*   **Build Tools:** `g++`, `cmake`, `make`.
*   **Scripting Environments:** `python3`, `Rscript`.
*   **Version Control:** Git.

### Installation

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd <project-directory>
    ```

2.  **Configure the environment:**
    Create a `prismquanta.env` file at the root of the project to define all shared paths (queues, logs, repo locations). All scripts will source this file.

3.  **Build the entire ecosystem:**
    A top-level `build_all.sh` script orchestrates the build process for all components.
    ```bash
    ./build_all.sh
    ```

## Usage

A master control script will launch all necessary components in the correct order.

```bash
./start_system.sh
```

The system uses a file-based queue for communication between components. For details on how to interact with the system (e.g., adding tasks), please refer to the documentation for `QuantaLista`.

## Testing

A unified testing script will run unit and integration tests for each module.

```bash
./test_all.sh
```

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
