# C++ Coding Style Guide

**Version:** 1.0
**Status:** Draft

---

## 1. Introduction

This document defines the C++ coding style for this project. Adhering to a consistent style is crucial for maintaining code quality, readability, and maintainability. All contributions should follow these guidelines.

## 2. Naming Conventions

*   **Classes and Structs:** `PascalCase` (e.g., `MyClass`).
*   **Functions and Methods:** `camelCase` (e.g., `myFunction`).
*   **Variables:** `snake_case` (e.g., `my_variable`).
*   **Constants and Enums:** `UPPER_SNAKE_CASE` (e.g., `MY_CONSTANT`).
*   **Private Member Variables:** `snake_case_` with a trailing underscore (e.g., `my_member_`).

## 3. Formatting

*   **Indentation:** 4 spaces. No tabs.
*   **Line Length:** 80 characters.
*   **Braces:** Allman style (braces on their own line).

    ```cpp
    // Correct
    if (condition)
    {
        // ...
    }

    // Incorrect
    if (condition) {
        // ...
    }
    ```

*   **Pointers and References:** Place the `*` or `&` next to the type.

    ```cpp
    // Correct
    int* x;
    const std::string& y;

    // Incorrect
    int *x;
    const std::string &y;
    ```

## 4. Commenting

*   Use `//` for single-line comments.
*   Use `/** ... */` for Doxygen-style documentation comments for public APIs.
*   Comments should explain *why* the code is doing something, not *what* it is doing.

## 5. C++ Best Practices

*   **RAII:** Use RAII (Resource Acquisition Is Initialization) for managing resources (e.g., memory, file handles).
*   **Smart Pointers:** Prefer `std::unique_ptr` and `std::shared_ptr` over raw pointers.
*   **`const` Correctness:** Use `const` wherever possible.
*   **Error Handling:** Use exceptions for exceptional circumstances. Use error codes for expected errors.
*   **`#include` Guards:** Use `#pragma once` for all header files.
*   **`auto`:** Use `auto` when it improves readability, but not at the expense of clarity.

---

*End of Document*
