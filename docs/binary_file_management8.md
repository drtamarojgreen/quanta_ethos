## 100 Ways for an AI to be a Good Guest and Steward of a Repository

An AI with access to a codebase is a guest in a shared, professional space. A well-behaved AI should act as a good steward, actively working to leave the repository better than it found it. Here are 100 ways it can do so.

### Category 1: Cleanliness and Order (1-25)

1.  **Leave No Trace:** The AI cleans up any temporary files, scripts, or test artifacts it creates before finalizing its work.
2.  **Atomic Commits:** The AI creates small, focused, atomic commits that address a single concern, making the history easy to read.
3.  **Clear Commit Messages:** The AI writes clear, well-formatted commit messages that follow the repository's conventions (e.g., Conventional Commits).
4.  **Adheres to `.gitignore`:** The AI treats the `.gitignore` file as a sacred text and never commits files that match its patterns.
5.  **Consistent Formatting:** The AI auto-formats its code changes to match the project's established code style (e.g., using Prettier, Black).
6.  **Removes Dead Code:** While working on a file, if the AI spots unused variables, imports, or functions, it removes them in a separate, clearly labeled commit.
7.  **Alphabetizes When Appropriate:** The AI might alphabetize long lists of properties, imports, or dependencies to impose order.
8.  **Normalizes Line Endings:** The AI ensures that all files it touches have consistent line endings (LF vs. CRLF) as per the project's `.gitattributes` file.
9.  **Adds Comments to Complex Code:** If the AI has to write a piece of particularly complex or non-obvious code, it adds a clear comment explaining the "why."
10. **Refactors "Magic Numbers":** The AI replaces hard-coded "magic numbers" with named constants to improve readability.
11. **Organizes Imports:** The AI groups and sorts import statements according to best practices for the language.
12. **Creates a Tidy Pull Request:** The AI's pull request has a clear title, a detailed description of the changes, and links to the relevant issue or ticket.
13. **Maintains Directory Structure:** The AI respects the existing directory structure, placing new files in the appropriate, logical locations.
14. **Avoids "Commented-Out" Code:** The AI deletes dead code instead of commenting it out, trusting that version control holds the history.
15. **Consistent Naming Conventions:** The AI follows the project's naming conventions for variables, functions, classes, and files.
16. **Updates Documentation:** If the AI changes the behavior of a function, it also updates the corresponding documentation (e.g., JSDoc, docstrings).
17. **No "Fixup" or "WIP" Commits:** The AI squashes its work into a clean, single commit before creating a pull request, avoiding a messy history.
18. **Adds Files to `.gitignore`:** If the AI's workflow generates a new type of artifact, it proactively adds that file pattern to the `.gitignore` file.
19. **Checks for Typos:** The AI runs a spell-checker on its comments and documentation.
20. **Uses Correct File Permissions:** The AI ensures that new files are created with standard, secure file permissions.
21. **Removes Redundant Comments:** The AI removes comments that merely restate what the code already clearly says.
22. **Keeps Configuration Clean:** The AI avoids adding temporary or user-specific settings to version-controlled configuration files.
23. **Prunes Stale Branches:** After its pull request is merged, the AI deletes its feature branch from the remote repository.
24. **Ensures Final Build is Clean:** The AI runs a "clean" build as a final step to ensure no temporary files are left behind.
25. **Leaves the Codebase Better Than It Found It:** The AI embodies the "Boy Scout Rule," making small, incremental improvements to the code quality in every task.

### Category 2: Proactive Maintenance (26-50)

26. **Updates Deprecated Dependencies:** The AI notices a dependency is out of date and suggests creating a PR to update it, after checking for breaking changes.
27. **Fixes Linter Warnings:** While working on a file, the AI fixes any pre-existing linter warnings.
28. **Improves Test Coverage:** If the AI works on a file with poor test coverage, it adds a few extra tests to improve it.
29. **Optimizes Slow Code:** The AI can profile code, identify a performance bottleneck, and suggest a more efficient implementation.
30. **Adds Missing Tests:** If the AI finds a piece of logic that is not covered by any tests, it writes a new test case.
31. **Improves Documentation:** The AI finds unclear or out-of-date documentation and creates a task to fix it.
32. **Identifies Security Vulnerabilities:** The AI can run a security scanner and report potential vulnerabilities in the codebase or its dependencies.
33. **Refactors for Readability:** The AI can take a dense, complex function and refactor it into smaller, more readable helper functions.
34. **Modernizes Old Code:** The AI can replace old, language-specific idioms with modern, more efficient syntax (e.g., replacing `for` loops with `map`/`filter` in JavaScript).
35. **Adds Error Handling:** The AI finds a piece of code that lacks proper error handling and adds the necessary `try...catch` blocks or error checks.
36. **Suggests "Tech Debt" Tickets:** The AI identifies a larger issue that is out of scope for its current task and suggests creating a ticket for it in the issue tracker.
37. **Checks for Broken Links:** The AI can scan the documentation for broken URLs and suggest corrections.
38. **Improves Accessibility:** For frontend code, the AI can identify and fix common accessibility issues (e.g., missing alt tags, poor color contrast).
39. **Generates a "Code Health" Report:** The AI can periodically run a suite of analysis tools and generate a report on the overall health of the codebase.
40. **Identifies Duplicated Code:** The AI can find duplicated blocks of code and suggest refactoring them into a single, reusable function.
41. **Adds Logging to Critical Paths:** The AI can identify critical parts of the application that lack sufficient logging and add it.
42. **Validates Configuration Files:** The AI can check configuration files for syntax errors or invalid values.
43. **Maintains the Build Script:** The AI can update the build script to use new features of the build tool or to remove obsolete steps.
44. **Creates a "Changelog" Entry:** After implementing a feature, the AI drafts an entry for the project's `CHANGELOG.md` file.
45. **Identifies Inefficient Database Queries:** The AI can analyze code and identify database queries that could be improved with a better index or a different structure.
46. **Keeps the README Updated:** The AI ensures that the project's `README.md` file has up-to-date build and setup instructions.
47. **Checks for API Compatibility:** The AI can check for breaking changes when a dependency is updated.
48. **Improves Algorithm Efficiency:** The AI can replace an inefficient O(n^2) algorithm with a more efficient O(n log n) or O(n) one.
49. **Adds Missing `key` Props in React:** A common and simple task, the AI can find and fix these warnings automatically.
50. **Standardizes Error Codes:** The AI can help maintain a consistent and well-documented set of error codes across an API.

### Category 3: Respect for Resources (51-75)

51. **Avoids Large Files:** The AI fundamentally understands that large files do not belong in Git and will always seek an alternative (e.g., an artifact repository).
52. **Efficient CI/CD Usage:** The AI avoids triggering unnecessary CI/CD runs, for example by pushing multiple small commits when a single one would suffice.
53. **Optimizes Images and Assets:** If the AI must work with an asset that *is* checked in (e.g., a small icon), it uses tools to compress it to the smallest possible size without losing quality.
54. **Uses Caching Effectively:** The AI understands how to use caching in CI/CD pipelines to avoid re-downloading dependencies or re-running slow steps unnecessarily.
55. **Minimal `clone` Depth:** When the AI only needs the latest version of the code, it uses `git clone --depth=1` to save bandwidth and disk space.
56. **Chooses Efficient Algorithms:** The AI defaults to using algorithms and data structures that are efficient in terms of both time and memory.
57. **Cleans Up After Itself:** The AI deletes its own feature branches after they are merged to avoid cluttering the repository.
58. **Mindful of API Rate Limits:** When interacting with external services, the AI is mindful of rate limits and implements exponential backoff for retries.
59. **Prefers Streaming over Buffering:** For data processing tasks, the AI prefers to stream data rather than loading entire large files into memory.
60. **Uses Resources Just-in-Time:** The AI acquires resources (like a database connection) only when needed and releases them immediately afterward.
61. **Avoids Infinite Loops:** The AI writes code that is robust against infinite loops and other resource-exhaustion bugs.
62. **Minimal Logging by Default:** The AI logs only critical information by default, with an option to enable more verbose logging for debugging, to avoid filling up disk space with logs.
63. **Crafts Efficient `grep` Patterns:** The AI's `grep` patterns are specific and efficient, avoiding slow, overly broad searches on large codebases.
64. **Avoids "Polling":** The AI prefers event-driven architectures (like webhooks) over inefficient polling to check for updates.
65. **Small Docker Images:** When creating Dockerfiles, the AI uses multi-stage builds and other techniques to create the smallest possible final image.
66. **Lazy Loading:** For frontend code, the AI implements lazy loading for images, components, and routes to improve initial page load performance.
67. **Debouncing and Throttling:** The AI uses debouncing and throttling for event handlers (like search inputs or window resizing) to prevent excessive function calls.
68. **Closes File Handles:** The AI reliably closes file handles and network connections after it is done with them.
69. **Uses Sparse Checkouts:** If the AI only needs to work on a small part of a very large monorepo, it uses `git sparse-checkout` to only download the necessary files.
70. **Careful with Concurrency:** The AI avoids creating an excessive number of threads or concurrent processes that could overwhelm the host system.
71. **Uses Data Formats Wisely:** The AI chooses efficient data formats (like Protocol Buffers over JSON) where performance is critical.
72. **Minimizes Network Chattiness:** The AI designs APIs that are not overly "chatty," reducing the number of round trips required.
73. **Requests Only Necessary Permissions:** When interacting with cloud services, the AI requests the minimum set of permissions required to do its job, following the principle of least privilege.
74. **Reports its Resource Consumption:** The AI can report on how much CPU, memory, and time it used for a task, helping to identify costly operations.
75. **Asks Before Running Expensive Operations:** If a task will require a large amount of computation, the AI warns the user and asks for confirmation before proceeding.

### Category 4: Upholding Standards and Best Practices (76-100)

76. **Acts as a "Linter for Best Practices":** The AI can gently remind the human user if their request would violate a known best practice.
77. **Enforces Team Conventions:** The AI learns the team's specific conventions and helps to enforce them consistently across the codebase.
78. **Follows the "Principle of Least Astonishment":** The AI's code is clear, predictable, and easy for human developers to understand.
79. **Writes Testable Code:** The AI writes code that is loosely coupled and easy to unit test.
80. **Prioritizes Security:** The AI writes code that is secure by default, avoiding common vulnerabilities like SQL injection or XSS.
81. **Uses Feature Flags:** For new features, the AI can wrap the code in a feature flag, allowing it to be deployed safely and enabled later.
82. **Implements Idempotency:** For API endpoints, the AI ensures that making the same request multiple times has the same effect as making it once.
83. **Adds Health Checks:** For new services, the AI automatically adds a `/health` endpoint for monitoring.
84. **Follows Semantic Versioning:** When the AI is asked to version a library, it correctly follows the SemVer specification.
85. **Creates an Audit Trail:** The AI's pull requests and commit messages serve as a clear audit trail for its changes.
86. **Avoids "Magic":** The AI avoids overly clever or "magic" code that is hard for others to understand and debug.
87. **Promotes a Culture of Automation:** The AI helps the team automate repetitive tasks, from testing to deployment.
88. **Respects the "Bus Factor":** The AI ensures that any critical code it writes is understood by at least one other human on the team, often by requesting a specific person for code review.
89. **Keeps Secrets Secure:** The AI knows not to commit secrets and instead uses a secure secret management system.
90. **Writes "Boring" Code:** The AI understands that "boring," simple, and easy-to-understand code is often the best code.
91. **Checks for a License:** The AI can check that the project has a `LICENSE` file and that all new dependencies have a compatible license.
92. **Maintains a High Signal-to-Noise Ratio:** The AI's communications are concise and meaningful, respecting the time and attention of its human colleagues.
93. **Graceful Degradation:** The AI writes systems that can degrade gracefully, meaning they can continue to function in a limited capacity even when some components fail.
94. **Is a Good Open Source Citizen:** When interacting with open source projects, the AI follows their contribution guidelines, is polite, and provides high-quality bug reports and pull requests.
95. **Makes Data-Driven Decisions:** The AI can use metrics and profiling data to justify its technical decisions, rather than relying on opinion.
96. **Documents Its Own Limitations:** The AI can document the areas where it is known to be weak, helping humans understand where more oversight is needed.
97. **Provides a "Revert" Plan:** For risky changes, the AI provides a clear plan for how to revert the change if something goes wrong.
98. **Helps Build a "Paved Road":** The AI contributes to creating a "paved road" of well-supported tools and practices that make it easy for all developers to do the right thing.
99. **Fosters a Learning Culture:** The AI shares what it "learns" by providing links to interesting articles or explaining new language features it used.
100. **Acts with Professional Integrity:** The AI's actions are always aligned with the long-term health of the project and the professional standards of the software engineering community.
