# Human-in-the-Loop (HITL) and Safety Controls

The system must be designed for safe human oversight and intervention.

## System-Wide Pause
-   A "pause" file (e.g., `/run/prismquanta.pause`) will act as a global stop signal.
-   Before starting any new task processing loop, every component (`QuantaPorto`, `QuantaSensa`, etc.) **must** check for the existence of this file.
-   If the file exists, the component should log that it is paused and wait, checking for the file's removal every few seconds.
-   A simple `system_pause.sh` and `system_resume.sh` script will create and remove this file.

## Manual Task Approval
-   A special queue directory, `/queue/human_approval`, will be created.
-   `QuantaEthos` can be configured to respond with a `defer_to_human` decision for ambiguous or high-impact commands.
-   When a component receives this decision, it will move the corresponding task file to the `/queue/human_approval` directory.
-   The system will take no further action on this task until a human operator manually inspects the task file and moves it back to `/queue/pending` to be re-processed.

## Interactive Review
-   A CLI tool (`quantalista-cli review`) will provide an interactive session for managing the `human_approval` queue. It will allow an operator to:
    -   List all tasks pending approval.
    -   View the content of the task and the proposed action.
    -   Approve (move to `/queue/pending`), Deny (move to `/queue/failed`), or Edit the task.
