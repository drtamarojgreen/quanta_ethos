# Troubleshooting Guide

**Version:** 1.0
**Status:** Draft

---

## 1. Introduction

This guide provides solutions to common problems you may encounter when building, running, or using the C++ LLM application.

## 2. Build and Compilation Issues

### 2.1. CMake Error: Could not find a package configuration file for `vcpkg`

**Problem:** CMake is unable to find the `vcpkg` toolchain file.

**Solution:** Make sure you have correctly set the `CMAKE_TOOLCHAIN_FILE` variable when running CMake. The path should point to the `vcpkg.cmake` file in your `vcpkg` installation directory.

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
```

### 2.2. Compilation Fails with "Header not found" Errors

**Problem:** The compiler cannot find the header files for the project's dependencies.

**Solution:** Ensure that you have installed all the required dependencies using `vcpkg` as described in the `developer_guide.md`. If you have already installed them, try reinstalling them to ensure they are correctly integrated with your build system.

## 3. Runtime Issues

### 3.1. Application Fails to Start with "Model not found" Error

**Problem:** The application cannot find the LLM model file.

**Solution:** By default, the application looks for the model file at `/app/models/model.onnx`. Make sure the model file exists at this location. You can also specify a different path using the `MODEL_PATH` environment variable.

### 3.2. API Requests Return 500 Internal Server Error

**Problem:** The API server is encountering an unhandled exception.

**Solution:** Check the application logs for more information about the error. You can set the `LOG_LEVEL` environment variable to `debug` to get more detailed logs.

## 4. Getting Help

If you encounter a problem that is not listed here, please open an issue on our GitHub repository. Provide as much detail as possible, including:

*   The version of the application you are using.
*   The steps to reproduce the problem.
*   The full error message and any relevant logs.

---

*End of Document*
