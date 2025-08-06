# Security and Environment

## Dependency and Environment Management

A consistent and reproducible environment is key to operational efficiency and stability.

### Centralized Configuration
-   The `prismquanta.env` file is the **single source of truth** for all system paths, model locations, and key operational parameters (e.g., queue retention periods).
-   All scripts and executables **must** source this file at startup to load their configuration. Hardcoding paths is strictly forbidden.

### Reproducible Builds
-   **Python:** Each Python component will have a `requirements.txt` file. The `build_all.sh` script will create a dedicated Python virtual environment (`venv`) for each component and install its specific dependencies, a new, empty log file will be created.
-   **C++:** All C++ components will use `cmake`. The top-level build script will iterate through them, running a standard `cmake . && make` sequence and placing the final binaries into a central `${PRISMQUANTA_ROOT}/bin` directory.
-   **Environment Validation:** A `check_system.sh` script will be provided. It will run before a full system start to verify that all required tools (`g++`, `python3`, `Rscript`), directories, models, and dependencies are present and correctly configured, preventing runtime failures due to setup issues.

## Security and Sandboxing Posture

Efficient resource management is intrinsically linked to security. The system must prevent any single component from consuming excessive resources or performing unauthorized actions.

### Principle of Least Privilege
-   **Dedicated User:** All PrismQuanta components should run under a dedicated, low-privilege user account (e.g., `prismquanta`).
-   **File Permissions:** The queue and log directories will have strict permissions. For example, the `QuantaSensa` process will only have write access to the `/actions` and `/queue/completed|failed` directories, but not to `/queue/pending`.

### Execution Sandboxing for QuantaSensa
The `QuantaSensa` C++ parent controller is the primary enforcement point for sandboxing and will apply resource limits *before* executing any action script.
-   **Resource Limits (cgroups):** On Linux, the parent controller will use `cgroups` to create a transient slice for each action it executes. This will enforce hard caps on CPU usage (e.g., `CPUQuota=50%`) and memory (e.g., `MemoryMax=512M`), preventing a faulty script from destabilizing the host.
-   **Filesystem Isolation (Namespaces):** The parent controller can use `mount` namespaces to provide the action script with a restricted view of the filesystem. It can create a temporary, private `/tmp` directory and only bind-mount the specific project directories the task needs to access.
-   **Configuration Immutability:** Critical configuration files (e.g., `prismquanta.env`, `alarms.yaml`) will be owned by a root/admin user, with read-only permissions for the `prismquanta` user. This prevents an agent from modifying its own operational parameters or safety rules.
