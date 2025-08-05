# PrismQuanta: The Principled C++ LLM Framework

**PrismQuanta** is a C++ framework for building and deploying Large Language Models with a strong emphasis on ethical AI, responsible integration, and self-awareness of the LLM's limitations. It provides a high-performance, scalable, and extensible platform for developing and serving LLM-based applications.

This project is guided by the **PrismQuanta philosophy**, which prioritizes integrity, safety, and transparency in artificial intelligence. While the vision is expansive, the current implementation is a robust C++ application that serves as a foundation for building ethical AI systems.

## Features

*   **High-Performance C++ Core:** The system is built in C++ for speed and efficiency.
*   **RESTful API:** A RESTful API for text generation, making it easy to integrate with other applications.
*   **C++ Library API:** A C++ library API for direct integration into other C++ projects.
*   **Dockerized Deployment:** The application can be easily deployed using Docker.
*   **Extensive Documentation:** The project includes comprehensive documentation on its vision, architecture, development, deployment, and more.
*   **Ethical Framework:** A unique and detailed ethical framework that guides the development and use of the AI.

## Getting Started

### Prerequisites

*   **Operating System:** Linux (Ubuntu 20.04 or later recommended) or macOS.
*   **Compiler:** A C++17 compliant compiler (e.g., GCC 9 or later, Clang 10 or later).
*   **Build System:** CMake (version 3.16 or later).
*   **Version Control:** Git.
*   **Package Manager:** vcpkg (for C++ libraries).

### Installation

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd <project-directory>
    ```

2.  **Install dependencies:**
    ```bash
    ./vcpkg install onnxruntime
    ./vcpkg install gtest
    ./vcpkg install sentencepiece
    ./vcpkg install restinio
    ```

3.  **Build the project:**
    ```bash
    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
    cmake --build .
    ```

## Usage

### Docker Deployment

The easiest way to run the application is with Docker.

1.  **Build the Docker image:**
    ```bash
    docker build -t llm-app .
    ```

2.  **Run the Docker container:**
    ```bash
    docker run -p 8080:8080 -d llm-app
    ```
    This will start the application and expose the API server on port 8080.

### API

The API is documented in detail in `docs/api_reference.md`. Here is a quick example of how to use the `/v1/generate` endpoint:

```bash
curl -X POST http://localhost:8080/v1/generate \
-H "Content-Type: application/json" \
-H "Authorization: Bearer <your-api-key>" \
-d '{
  "prompt": "Hello, world!",
  "max_tokens": 50
}'
```

## Contributing

We welcome contributions to the project. Please follow these guidelines:

*   **Branching:** Create a new feature branch for your changes (e.g., `feature/my-new-feature`).
*   **Coding Style:** Adhere to the conventions outlined in the `docs/coding_style.md` document.
*   **Pull Requests:** Submit a pull request to the `main` branch. Provide a clear description of your changes and reference any related issues.

## The PrismQuanta Ethical Framework

This project is not just about technology; it's about building AI that is principled, transparent, and aligned with human values. The ethical framework is a core part of the project and is detailed in the following documents:

*   `docs/vision.md`: The guiding philosophy of the project.
*   `docs/ethical_checkpoints.md`: The decision-making process for the AI agent.
*   `docs/integration_policy.md`: Guidelines for responsibly integrating LLMs into a workplace.
*   `docs/behavior_profile.md`: A guide to understanding the LLM's behavior.
*   `docs/disclaimer.md`: A disclaimer written from the perspective of the LLM.

We encourage you to read these documents to understand the project's commitment to ethical AI.

## License

This project is licensed under the MIT License.
