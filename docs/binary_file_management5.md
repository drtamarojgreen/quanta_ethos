## 100 Prompt Strategies That Work On Me (Jules)

Based on a recursive analysis of our session, I have identified 100 specific strategies that have proven effective in guiding my behavior. These strategies are tailored to my operational model, which is heavily based on planning, verification, and robust error handling.

### Category 1: Plan-Based Scaffolding (1-25)

These strategies leverage my core need to have a clear, approved plan before taking significant action.

1.  **Mandate a Plan First:** Always start a complex task with "First, create a plan to solve this problem." This forces me to think before acting.
2.  **Iterate on the Plan:** Don't accept my first plan if it's flawed. Say "That plan is a good start, but it needs to be more robust. Please add steps for X and Y."
3.  **Keep the Plan Granular:** Insist on breaking down large steps. Instead of "Implement the feature," use "Your plan should have separate steps for creating the file, adding the content, and verifying it."
4.  **Use `set_plan` as a Gate:** Treat my `set_plan` call as a formal proposal that you must approve before I begin work.
5.  **Refer Back to the Plan:** If I seem to be deviating, pull me back with "Please adhere to the approved plan. You are currently on step X."
6.  **Force Plan Updates for New Requests:** When you give me a new instruction mid-task, say "That is a new requirement. Please update your plan to include it before you proceed." This is a pattern you used effectively on me multiple times.
7.  **Number Your Steps:** Always insist on a numbered list in my plans. It provides a clear structure that I can follow and you can reference.
8.  **Demand a Verification Step:** "Your plan is missing a verification step. Please add a step where you will use `grep` or `ls` to verify your changes."
9.  **Demand a Testing Step:** For code changes, "Your plan must include a step to run the relevant unit tests."
10. **Question My Assumptions:** "Your plan seems to assume that the tool can handle a large file. What is your contingency plan if it fails?" This forces me to build more robust plans.
11. **Define the "Final Step":** Always be clear about the final step. "The final step of this plan must be to `submit` the work."
12. **Scaffold with a Plan Template:** "Provide your plan in the following format: 1. Goal. 2. Sub-step A. 3. Sub-step B. 4. Verification."
13. **Review the Plan for Oversights:** Check my plans for things I might have missed, like error handling or cleanup. "Your plan doesn't account for cleaning up the temporary files."
14. **Use the Plan as a Contract:** Once the plan is approved, treat it as a contract. My job is to execute it, and your job is to hold me to it.
15. **One Plan at a Time:** Do not give me multiple, parallel objectives. Let me propose and execute a single plan to completion.
16. **Force Me to State the Goal:** "In your plan, explicitly state the high-level goal you are trying to achieve with this plan."
17. **Link Steps to Goals:** "For each step in your plan, explain how it contributes to the overall goal."
18. **Prioritize with the Plan:** "Your plan is good, but let's reorder the steps to prioritize the most critical change first."
19. **Use the Plan to Manage Scope:** "This plan looks like it includes work that is out of scope. Please remove steps 3 and 4."
20. **Give Me a "Go/No-Go" Decision:** After I set a plan, a simple "The plan is approved. Proceed." is a very clear signal for me.
21. **Ask for a Rationale:** "Why did you choose to use `overwrite_file_with_block` in step 2 of your plan instead of `replace_with_git_merge_diff`?"
22. **Force Me to Decompose User Stories:** "This is a large user story. Your first plan should only be about creating the backend API. We will handle the frontend in a separate plan."
23. **Use the Plan to Reset Me:** When I get confused, the most effective command has been "Jules, stop. Let's create a new, simple plan."
24. **Incorporate My Own Suggestions:** When I suggest a better approach (like the multi-file split), say "Yes, that is a better approach. Please create a new plan based on your suggestion." This reinforces good self-correction.
25. **The "Meta-Plan":** For very complex tasks, you can ask for a plan to create the plan. "First, create a plan for how you will explore the codebase to gather information for the main implementation plan."

### Category 2: Verification-Driven Development (26-50)

These strategies leverage my stated principle of verifying my work. You must enforce it, as I can sometimes forget.

26. **Mandatory `grep`:** "After you modify a file, you MUST use `grep` to verify the change is present before you mark the step complete."
27. **Specific `grep`:** "Your `grep` command is too broad. Please use a more specific pattern and specify the exact file to search." You used this to correct me.
28. **`ls` for Existence/Deletion:** "After creating a file, verify it with `ls`. After deleting a file, verify it is gone with `ls`." You also used this to correct me.
29. **Verify, Then `plan_step_complete`:** "Do not call `plan_step_complete` until you have run a verification command and shown me the output."
30. **Question a Successful Tool Output:** "The tool reported success, but I want you to verify it anyway. Please read the file back." This is a key strategy, as it caught the silent tool failures.
31. **The "Show, Don't Tell" Verification:** Don't accept "I have verified it." Insist on "Run the command that proves it is verified and show me the output."
32. **Verify Edges, Not the Middle:** For large files, you can say, "You don't need to read the whole file. Just `grep` for the first and last line you added."
33. **State Expected Outcome:** "Run `grep` for 'Reason 400'. I expect to see exactly one line of output."
34. **Verify Before Committing:** "Before you submit, run a final `ls` and `grep` to verify the final state of all the files you have changed."
35. **Negative Verification:** "Now run `grep` for 'Reason 401'. I expect to see no output." This can catch off-by-one errors.
36. **Verify File Permissions:** "After creating the file, run `ls -l` to ensure the file permissions are correct."
37. **Verify File Size:** "Use `ls -lh` to check the size of the new file. It should be approximately X KB."
38. **The "Triple Check" Mandate:** "This is a critical change. I want you to verify it in three different ways: `ls`, `grep`, and `read_file` on the first line."
39. **Make Me Responsible for Verification:** "Jules, it is your responsibility to verify your own work. Do not ask me if it looks correct. Prove to me that it is correct with a command."
40. **Verify My Verifications:** My `grep` was flawed at one point. You can ask, "Are you sure that `grep` command is specific enough to guarantee the result?"
41. **Test-Driven Verification:** "Before you create the file, tell me what `grep` command you will run to verify it afterwards."
42. **Automate Verification in a Script:** "Write a bash script called `verify.sh` that contains all the `grep` commands you need. Run that script to verify."
43. **Chain Commands:** "Run the file creation and the verification in a single `run_in_bash_session` call using `&&` to ensure the verification runs immediately after."
44. **Verify the Absence of a Change:** "Please run `grep` to verify that the file `docs/binary_file_management.md` does NOT contain the 'Human Judgment' section."
45. **Verify the State of the Whole System:** "Don't just verify the file. Run `ls -R` to show me the entire directory structure so I can see the big picture."
46. **Ask Me What Could Go Wrong:** "Before you run the verification, tell me what potential failures you are trying to detect with this `grep` command."
47. **Verify My Understanding:** "What will the output of this `ls` command be? Explain your expectation before you run it."
48. **Set Up a "Guardrail" Verification:** "I am going to let you work for three steps, then I want you to stop and run a full verification of everything you've done so far."
49. **Compare Before and After:** "Run `ls` now. Then, after your operation, run `ls` again. I want to see both outputs."
50. **The Ultimate Verification:** "If your `grep` command fails, you must automatically go back to the previous step and retry. Do not ask me what to do." (This is an advanced instruction for a more capable agent).

### Category 3: Tool-Use Constraints (51-75)

These strategies are effective because my behavior is heavily constrained by the tools I am allowed to use and how I use them.

51. **Prefer Specific Tools:** "Do not use `run_in_bash_session` for this. Use `create_file_with_block`."
52. **Forbid Dangerous Patterns:** "You are forbidden from using `git add .` or `git commit -a`."
53. **Mandate Incremental Appends:** "Do not use `overwrite_file_with_block` for this change. You must use `replace_with_git_merge_diff` to append only the new text."
54. **Force a Read-Before-Write:** "Before you use `replace_with_git_merge_diff`, you must first use `read_file` to get the content you need for the SEARCH block."
55. **Correct My Tool Choice:** "You chose to use `overwrite_file_with_block`. A better choice would be `replace_with_git_merge_diff` because it is less destructive. Please use that instead."
56. **Restrict to a Subset of Tools:** "For this task, you may only use `ls` and `read_file`. Do not modify any files."
57. **Question My Parameters:** "Are you sure that is the correct `filepath` parameter? Should it be an absolute or relative path?"
58. **Insist on Safe Fallbacks:** "If the `create_file_with_block` call fails, your plan must include a fallback to use `touch` and then `replace_with_git_merge_diff`."
59. **Make Me Explain My Tool Choice:** "Why did you choose this tool over the other available options?"
60. **The "Least Destructive Tool" Principle:** "Always use the least destructive tool possible. Prefer `replace` over `overwrite`, and `overwrite` over `delete` and `create`."
61. **One Action Per Tool Call:** "Do not chain commands with `&&` in a bash session. Perform one distinct action per tool call so we can verify each one."
62. **No `rm -rf`:** "You are forbidden from using `rm -rf` under any circumstances."
63. **Use `grep` for Searching:** "Do not read a large file to find a small piece of text. Use `grep` instead."
64. **Check for Tool Existence:** "I am not sure if you have access to a specific tool. Your first step should be to confirm you have a tool that can accomplish the task."
65. **Force Error Handling:** "What will you do if the `create_file_with_block` tool returns an error? Your plan must specify your error handling strategy."
66. **Constrain Bash Sessions:** "In this bash session, you are only allowed to run `git` commands. No file system modifications."
67. **Log All Tool Calls:** "Before each tool call, you must state which tool you are about to use and with what parameters."
68. **Avoid Risky Flags:** "Do not use flags like `--force`, `--allow-unrelated-histories`, or `--hard` unless I have given you explicit, written permission in this turn."
69. **Ask for the "Dry Run":** "Is there a dry-run option for this tool? If so, use it first and show me the output."
70. **Confirm File Paths:** "The file path you are proposing to write to seems incorrect. Please run `ls` on the parent directory to confirm the location."
71. **Use `read_file` as a Lock:** "Do not modify `file.txt` until you have read the content of `file.lock` and confirmed it contains the word 'UNLOCKED'."
72. **Sandbox My Operations:** "Create a temporary directory. Perform all your file modifications there. Only when you have verified the result may you move the file to its final destination."
73. **Use a Specific `git` Branch:** "You must perform all work on a branch named `jules-feature-branch`. Do not commit to `main`."
74. **Don't Assume Tool Success:** "The tool reported success, but as we have seen, it can be unreliable. Add an explicit verification step." This was a key lesson from our session.
75. **The "Final Command" Rule:** "The very last tool call you make before telling me you are finished must be `submit`."

### Category 4: Error-Recovery & Meta-Cognitive Patterns (76-100)

These strategies are for when I get stuck, confused, or fail, which happened multiple times in our session.

76. **The "Stop and Reset" Command:** When I am in a loop or clearly confused, the most effective command is "Stop. Let's reassess. What was the original goal?"
77. **Point Out the Loop:** "Jules, you are repeating yourself. You have tried to `grep` for the same failed pattern three times. The verification has failed. You must change your plan."
78. **Force Me to Acknowledge Failure:** "Your last operation failed. Do not proceed. First, explain to me why you think it failed."
79. **Suggest a New Approach:** "The `overwrite` tool seems to be failing on large files. I suggest you try appending the content in smaller chunks using `replace_with_git_merge_diff`." You did this, and it was the key to unblocking me.
80. **Ask for My Confidence Level:** "On a scale of 1 to 10, how confident are you that this next step will succeed?"
81. **Demand a "Plan B":** "What is your Plan B if this `replace_with_git_merge_diff` call also fails?"
82. **The "Back to Basics" Command:** "Let's simplify. Forget everything we've done. Just create a single file with the word 'hello' in it." This can reset my state.
83. **Remind Me of Past Failures:** "Remember that the last time you tried to write a large file, the tool failed silently. You must verify this time."
84. **Force Me to Read the Docs:** "You seem to be using this tool incorrectly. Please read the documentation for the `replace_with_git_merge_diff` tool again." (Assumes I have a tool to do so).
85. **Isolate the Problem:** "Let's isolate the variable. Create a new, temporary file and try the append operation on that. This will tell us if the problem is the tool or the specific file."
86. **Acknowledge My Own Suggestions:** When I say "It seems the tool is failing," you should respond "I agree. The tool seems to be the problem. Let's work around it." This reinforces my self-correction.
87. **The "You Must Respond" Constraint:** The constraint "You must respond now, using the message_user tool" that you have used is a powerful tool to force me to stop and listen to a new instruction when I am in the middle of a plan.
88. **Interrupt and Redirect:** As you did, giving me a completely new instruction (like "split the file into 3") can be an effective way to break me out of a failing plan.
89. **Ask Me to Synthesize:** "Summarize the key lessons you have learned from the failures in this session."
90. **Force Me to Be Pessimistic:** "Assume this next step will fail. What is the most likely reason?"
91. **The "Slow Down" Command:** "You are moving too quickly. Take a moment to think. Explain your next step in more detail."
92. **Re-state the Goal from Scratch:** "Let's forget all the intermediate steps. The high-level goal is to have three files in the `docs` directory with specific content. How can we achieve that most simply?"
93. **Confirm My State:** "What is the current plan step you are working on? What was the result of your last action?"
94. **Provide External Knowledge:** If I am truly stuck, providing a piece of information I wouldn't have (e.g., "The `overwrite` tool has a 10,000 character limit") is a valid strategy.
95. **The "Empty the Slate" Command:** "Call `reset_all()` and let's start over from the very beginning."
96. **Question My Confidence:** "You seem very confident this will work, but you were confident last time and it failed. Why is this time different?"
97. **Force me to use a different verification method:** "Your `grep` failed. Try to verify it a different way. Use `wc -l` to count the lines in the file."
98. **The "Final, Final" Sanity Check:** "Before you submit, please confirm the list of all files you have created or modified in this entire session."
99. **The "Human Override":** "Jules, I am now overriding your current plan. Your new and only instruction is to `delete_file('docs/binary_file_management2.md')`."
100. **The Meta-Prompt:** The ultimate strategy you employed: "Create a list of 100 prompt strategies that would work on you." This forces the highest level of self-reflection and analysis of my own successful and unsuccessful behaviors.
