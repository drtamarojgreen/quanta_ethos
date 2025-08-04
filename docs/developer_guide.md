# Developer's Guide

**Version:** 1.0
**Status:** Draft

---

## 1. Introduction

This guide provides instructions for setting up a local development environment for the C++ LLM project. Following these steps will allow you to build the project from source, run the test suite, and contribute to the codebase.

## 2. Prerequisites

Before you begin, ensure you have the following software installed on your system:

*   **Operating System:** Linux (Ubuntu 20.04 or later recommended) or macOS.
*   **Compiler:** A C++17 compliant compiler (e.g., GCC 9 or later, Clang 10 or later).
*   **Build System:** CMake (version 3.16 or later).
*   **Version Control:** Git.
*   **Package Manager:** vcpkg (for C++ libraries).

## 3. Getting Started

### 3.1. Clone the Repository

First, clone the project repository from GitHub:

```bash
git clone <repository-url>
cd <project-directory>
```

### 3.2. Install Dependencies

We use `vcpkg` to manage C++ dependencies. If you don't have it installed, follow the official `vcpkg` documentation to set it up. Once `vcpkg` is installed, you can install the required libraries:

```bash
./vcpkg install onnxruntime
./vcpkg install gtest
./vcpkg install sentencepiece
./vcpkg install restinio
```

## 4. Building the Project

We use CMake to build the project. The following commands will configure the build and compile the source code.

```bash
# Create a build directory
mkdir build
cd build

# Configure the project with CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake

# Build the project
cmake --build .
```

This will create the executables and libraries in the `build/` directory.

## 5. Running the Test Suite

We use GTest for unit testing. To run the tests, execute the following command from the `build/` directory:

```bash
ctest
```

All tests should pass. If any tests fail, please review the error messages and consult the `troubleshooting.md` guide.

## 6. Contribution Guidelines

We welcome contributions to the project. Please follow these guidelines when submitting code:

*   **Branching:** Create a new feature branch for your changes (e.g., `feature/my-new-feature`).
*   **Coding Style:** Adhere to the conventions outlined in the `coding_style.md` document.
*   **Pull Requests:** Submit a pull request to the `main` branch. Provide a clear description of your changes and reference any related issues.

---

*End of Document*
