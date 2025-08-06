# Monitoring and Observability Strategy

The central `events.jsonl` file is the backbone of system observability. Every significant action from any component should be logged here.

## `events.jsonl` Structure
Each line is a self-contained JSON object.
```json
{
  "timestamp": "2023-10-27T10:00:05Z",
  "component": "QuantaSensa",
  "level": "INFO",
  "event_type": "action_execution_start",
  "message": "Executing script for task-123.",
  "metadata": {
    "task_id": "task-123",
    "script_path": "/path/to/actions/in_progress/task-123.sh"
  }
}
```

## Monitoring Tools
-   **QuantaPulsa (Real-time Dashboard):** A terminal-based tool (e.g., using Python's `Textual` or `curses`) that tails `events.jsonl`. It will provide a live-updating dashboard showing:
    -   The current count of tasks in each queue (`pending`, `in_progress`, `failed`).
    -   A real-time stream of the latest events.
    -   A summary of recent errors.
    -   CPU/Memory usage of the host system.
-   **QuantaAlarma (Alerting Daemon):** A lightweight background service that also tails `events.jsonl`. It will read a configuration file (`alarms.yaml`) to trigger actions based on event patterns.
    -   **Example `alarms.yaml` rule:**
      ```yaml
      - name: "High-Risk Action Approved"
        condition:
          event_type: "ethos_validation_complete"
          metadata.trust_score: "< 0.5"
          metadata.decision: "approve"
        action: "send_email --to admin@local.host --subject 'Low-Trust Action Approved'"
      ```
