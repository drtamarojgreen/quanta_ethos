# Developer Guide

## 1. Phased Integration Roadmap

This section outlines the planned phases for integrating the PrismQuanta ecosystem.

### Phase 1: The Core Execution Loop (Sensa + Ethos + Porto)
1.  **Standardize Configuration:** Create a root `prismquanta.env` file to define all shared paths (queues, logs, repo locations). All scripts must source this file.
2.  **Package `QuantaEthos`:** Build `QuantaEthos` as a command-line tool that accepts a string and returns a JSON validation object.
3.  **Integrate `QuantaSensa` and `QuantaPorto`:**
    -   Modify `QuantaPorto` to generate a simple shell script for a given task.
    -   Modify `QuantaSensa`'s C++ parent controller to poll a directory for this script, execute it, and log the result.
    -   The parent controller **must** call the `QuantaEthos` CLI tool for validation before executing the script.

### Phase 2: High-Level Orchestration (Lista)
1.  **Implement `QuantaLista`:** Develop `QuantaLista` to manage the file-based task queue in `/queue/pending`. It should provide CLI commands to add, list, and prioritize tasks.
2.  **Connect `QuantaLista` to `QuantaPorto`:** `QuantaPorto` will now be driven by the tasks appearing in the queue managed by `QuantaLista`, forming the complete strategy-to-execution pipeline.
3.  **Implement Feedback Loop:** `QuantaLista` will monitor the `/queue/completed` and `/queue/failed` directories to track overall project progress.

### Phase 3: Self-Awareness and Knowledge Management (Glia)
1.  **Develop `QuantaGlia` Triggers:** `QuantaGlia` will be a scheduled process (e.g., cron job) that scans all project repositories and the central `events.jsonl` log.
2.  **Implement `QuantaGlia` Actions:** Based on its analysis (e.g., detecting repeated errors, identifying missing documentation), `QuantaGlia` will autonomously create new tasks and place them in `QuantaLista`'s queue.

### Phase 4: Full Ecosystem and Visualization
1.  **Integrate Specialized Modules:** Wrap modules like `QuantaDorsa` and `QuantaCogno` with simple CLI interfaces so they can be invoked as tools by `QuantaSensa` agents.
2.  **Activate Monitoring:** Develop `QuantaPulsa` and `QuantaAlarma` as services that tail the `events.jsonl` file to provide real-time system health monitoring and alerts.
3.  **Unify Visualization Tools:**
    -   **Short-Term:** Provide documentation for running Windows-based visualizers (`QuantaRetina`, `multiple_viewer`) in a container or VM.
    -   **Long-Term:** Re-implement the visualization logic using a cross-platform terminal library (e.g., `ncurses` in C++, `Textual` in Python) or as a simple local web application that reads data files.

## 2. Challenges and Mitigations

This section details potential challenges and the strategies to mitigate them.

### Challenge 1: Concurrency and Race Conditions
**Description:** With multiple components monitoring file-based queues, there is a risk of two processes attempting to claim the same task file simultaneously, leading to duplicated or lost work.
**Mitigation:**
-   **Atomic Operations:** Use atomic file system operations like `mv`. On POSIX-compliant systems, `mv` is an atomic operation within the same filesystem, ensuring that a file "move" is an all-or-nothing action.
-   **Locking Mechanism:** Before processing a file in `/queue/pending`, a component will attempt to create a corresponding `.lock` file (e.g., `task-123.json.lock`). If the lock file creation succeeds, the component has exclusive access. The lock file is deleted upon completion.

### Challenge 2: State Management and Crash Recovery
**Description:** If a component (e.g., `QuantaSensa`) crashes while a task is in the `/queue/in_progress` directory, the task will be orphaned and never completed.
**Mitigation:**
-   **Task Timeouts:** Each task JSON will include a `max_runtime_sec` field.
-   **Reaper Process:** A "reaper" function within `QuantaLista` will periodically scan the `/queue/in_progress` directory. If a task's file modification time exceeds its `max_runtime_sec`, it will be moved to a `/queue/failed` directory. A detailed failure log will be generated, noting the timeout.

### Challenge 3: Local LLM Performance Bottlenecks
**Description:** The synchronous validation call to `QuantaEthos` could become a significant bottleneck if the local LLM is slow, delaying every critical action across the system.
**Mitigation:**
-   **Model Tiering:** Use smaller, faster, quantized models specifically for `QuantaEthos`. The ethical validation task is often a classification or simple pattern-matching problem that doesn't require a large, powerful model.
-   **Prompt Engineering:** Design prompts for `QuantaEthos` to be extremely concise and require a simple, structured output (e.g., a single JSON object), minimizing token generation time.
-   **Caching:** Implement a simple cache (e.g., an in-memory map or a file-based key-value store) within `QuantaEthos`. If an identical command is validated multiple times within a short period, return the cached decision.

### Challenge 4: Cross-Platform Compatibility
**Description:** The ecosystem involves C++, Python, R, and shell scripts, intended to run on both Linux and Windows. Differences in file paths, shell commands (`rm` vs `del`), and system APIs pose a significant integration challenge.
**Mitigation:**
-   **Configuration Abstraction:** Strictly enforce the use of the `prismquanta.env` file for all paths. Scripts should source this file to get environment-specific locations.
-   **Containerization (Short-Term):** For Windows-specific components like the visualizers, the recommended deployment method will be within a container (Docker) or a Linux environment via WSL2. This isolates them from the host and provides a consistent Linux environment.
-   **Cross-Platform Tooling (Long-Term):** Prioritize re-implementing critical UI components using cross-platform technologies (e.g., a simple web server in Python with a web UI, or a terminal UI library like `Textual`).
-   **Conditional Logic in Scripts:** Where unavoidable, use conditional logic in shell scripts to detect the OS (`if [[ "$OSTYPE" == "linux-gnu"* ]]`) and execute the appropriate command.

### Challenge 5: Integration Testing Complexity
**Description:** Testing the interaction between multiple asynchronous components written in different languages is inherently difficult.
**Mitigation:**
-   **Mock Components:** Develop lightweight, mock versions of key executables for testing purposes. For example, a `mock_quanta_ethos` script that always returns an "approved" JSON, or a `mock_sensa_agent` that immediately creates a success result file.
-   **Dedicated Test Suite:** Create an integration test script (`test_integration.sh`) that:
    1.  Sets up a temporary queue directory structure.
    2.  Launches the components under test (e.g., `QuantaLista`, `QuantaPorto`).
    3.  Uses mock components for dependencies (like `QuantaSensa`).
    4.  Places a series of test task files into `/queue/pending`.
    5.  Asserts that the expected result files appear in `/queue/completed` or `/queue/failed` within a timeout.
    6.  Cleans up all temporary files and processes.

## 3. Component-Specific Integration Details

This section provides detailed integration information for each component.

### QuantaEthos (The Conscience)
-   **Interface:** Standalone CLI executable (`quanta-ethos`).
-   **Input:** A single string argument containing the command or action to be validated.
-   **Output:** A single JSON object to `stdout`.
-   **Implementation:**
    -   Must be extremely lightweight. It will be called frequently and should have minimal startup overhead.
    -   Will use a dedicated, highly quantized LLM optimized for classification and safety tasks.
    -   Will contain a non-LLM-based rules engine for immediate denial of high-risk patterns (e.g., `rm -rf /`, `> /dev/sda`).
    -   The `trust_score` will be a float between 0.0 and 1.0, allowing consuming services to set their own acceptance thresholds.

### QuantaPorto (The Commander)
-   **Primary Loop:**
    1.  Scan `/queue/pending` for the highest priority task file.
    2.  Use an atomic `mv` to move the task file to `/queue/in_progress`, effectively locking it.
    3.  Parse the task JSON.
    4.  Invoke its internal LLM to translate the natural language `description` into a structured PQL command or a shell script.
    5.  **Self-Validation:** Before finalizing, `QuantaPorto` can optionally call `quanta-ethos` on its generated script as a pre-check.
    6.  Write the final executable script to a new file in a `/actions/pending` directory, named with the same task ID (e.g., `task-123.sh`).
    7.  The original task file remains in `/queue/in_progress` as a record of the active job.

### QuantaSensa (The Field Agent)
-   **Parent Controller (C++) Loop:**
    1.  Scan `/actions/pending` for a script file.
    2.  Atomically move the script to `/actions/in_progress`.
    3.  Read the script content.
    4.  **Mandatory Validation:** Execute `quanta-ethos` with the script content as input. If the decision is `deny`, move the script to `/actions/failed` and create a corresponding failure result in `/queue/failed`. Abort this task.
    5.  If approved, execute the script, capturing `stdout` and `stderr`.
    6.  Upon completion, create a `result-123.json` file in `/queue/completed` (or `/queue/failed` if the script returned a non-zero exit code). The result file should contain the original task ID, status, and any captured output.
    7.  Delete the script from `/actions/in_progress` and the original task file from `/queue/in_progress`.

### QuantaLista (The Strategist)
-   **User Interface:** A simple CLI tool (`quantalista-cli`) will provide the primary human interface.
    -   `quantalista-cli add --priority high "Deploy the new monitoring script."` -> Creates a new task file in `/queue/pending`.
    -   `quantalista-cli list` -> Displays tasks in all queues.
    -   `quantalista-cli status <task_id>` -> Shows the current state and history of a task.
-   **Monitoring Daemon:** A background process will continuously monitor `/queue/completed` and `/queue/failed` to update a central state ledger (e.g., a simple SQLite database or a master JSON file) that tracks the history and outcome of all tasks.

### QuantaGlia (The Librarian)
-   **Trigger:** Will be executed via a system scheduler (e.g., a cron job) on a configurable interval (e.g., once every 6 hours).
-   **Execution Flow:**
    1.  Scan all repository directories defined in `prismquanta.env`.
    2.  Analyze git history, file sizes, and modification times.
    3.  Scan the central `events.jsonl` for patterns (e.g., repeated command failures).
    4.  Use its internal LLM to reason about potential improvements (e.g., "The `test_model_config.sh` script has failed 5 times with the same error. A task should be created to investigate.").
    5.  Generate a new task JSON file and place it directly into `QuantaLista`'s `/queue/pending` directory. The task description will contain the justification for its creation.
