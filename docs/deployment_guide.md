# Deployment Guide

**Version:** 1.0
**Status:** Draft

---

## 1. Introduction

This guide explains how to deploy the C++ LLM application to a production environment. It covers building a release version and deploying using Docker.

## 2. Building for Release

To create an optimized release build, use the following CMake commands:

```bash
# Create a build directory
mkdir build-release
cd build-release

# Configure the project with CMake for a release build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake

# Build the project
cmake --build .
```

This will generate a highly optimized executable in the `build-release/` directory.

## 3. Deploying with Docker

We provide a `Dockerfile` to simplify deployment. The `Dockerfile` builds the application in a container and sets up the necessary environment.

### 3.1. Building the Docker Image

To build the Docker image, run the following command from the project root:

```bash
docker build -t llm-app .
```

### 3.2. Running the Docker Container

To run the application in a Docker container, use the following command:

```bash
docker run -p 8080:8080 -d llm-app
```

This will start the application and expose the API server on port 8080.

## 4. Configuration

The application can be configured using environment variables. The following variables are available:

*   `API_PORT`: The port for the API server to listen on (default: `8080`).
*   `MODEL_PATH`: The path to the LLM model file (default: `/app/models/model.onnx`).
*   `LOG_LEVEL`: The logging level (e.g., `info`, `debug`, `error`).

You can set these environment variables when running the Docker container:

```bash
docker run -p 8080:8080 -e MODEL_PATH=/app/models/my-model.onnx -d llm-app
```

---

*End of Document*
