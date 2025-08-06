# Resource Management and System Efficiency

To ensure the system remains stable and performant in constrained environments, a strict resource management strategy is essential.

## Lightweight Process Management
-   **On-Demand Execution:** The core principle is to run processes only when needed. `QuantaSensa`'s single-action-per-invocation model is the primary example.
-   **Scheduled, Not Persistent, Daemons:** Resource-intensive analysis tasks, such as those performed by `QuantaGlia`, will not run as persistent daemons. Instead, they will be invoked periodically by a system scheduler like `cron`, perform their work, and exit.
-   **Minimalist Tooling:** Monitoring tools like `QuantaAlarma` will be simple scripts that use efficient file-watching utilities (e.g., `tail -f` or `inotifywait`) rather than heavy, continuously polling loops.

## LLM Inference Optimization
-   **Model Specialization and Tiering:** The system will not rely on a single, monolithic LLM. Instead, it will use a tiered approach defined in `prismquanta.env`:
    -   `ETHOS_MODEL_PATH`: A small, highly quantized model (e.g., 1.5B parameters, 4-bit quantization) for the frequent, low-latency validation checks in `QuantaEthos`.
    -   `PORTO_MODEL_PATH`: A mid-sized model for `QuantaPorto`'s more complex task-to-script translation.
    -   `GLIA_MODEL_PATH`: A larger, more capable model for `QuantaGlia`'s infrequent, high-level analysis and reasoning tasks.
-   **Inference Caching:** `QuantaEthos` will implement a cache for validation decisions. Before invoking the LLM, it will compute a hash of the command string and check a local cache (e.g., a SQLite database). If a recent, non-expired decision exists, it will be returned immediately, dramatically reducing redundant computations.

## Data Lifecycle and Pruning
-   **Log Rotation:** A scheduled "janitor" script will manage the `events.jsonl` file. When the file exceeds a configured size (e.g., 100MB), it will be compressed and timestamped (e.g., `events-2023-10-27.jsonl.gz`), and a new, empty log file will be created.
-   **Queue Archiving:** The same janitor script will periodically archive the contents of the `/queue/completed` and `/queue/failed` directories. Task results older than a configured retention period (e.g., 14 days) will be bundled into a compressed archive and removed from the active queue directories to prevent indefinite disk usage growth.
