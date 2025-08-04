# API Reference

**Version:** 1.0
**Status:** Draft

---

## 1. Introduction

This document provides a reference for the public-facing API of the C++ LLM system. The API is exposed via a RESTful interface, and the following sections detail the available endpoints, request formats, and response formats.

## 2. Authentication

All API requests must include an API key in the `Authorization` header:

`Authorization: Bearer <your-api-key>`

## 3. Endpoints

### 3.1. `POST /v1/generate`

This endpoint generates text based on a given prompt.

#### Request Body

*   `prompt` (string, required): The input text to generate from.
*   `max_tokens` (integer, optional, default: 100): The maximum number of tokens to generate.
*   `temperature` (float, optional, default: 0.7): The sampling temperature. Higher values result in more random outputs.

**Example:**

```json
{
  "prompt": "Hello, world!",
  "max_tokens": 50
}
```

#### Response Body

*   `id` (string): A unique identifier for the generation request.
*   `text` (string): The generated text.
*   `finish_reason` (string): The reason the generation finished (e.g., `length`, `stop_sequence`).

**Example:**

```json
{
  "id": "gen-12345",
  "text": "Hello, world! This is the generated text.",
  "finish_reason": "length"
}
```

### 3.2. `GET /v1/models`

This endpoint retrieves a list of available models.

#### Request Body

None.

#### Response Body

*   `models` (array of strings): A list of model names.

**Example:**

```json
{
  "models": [
    "model-a",
    "model-b"
  ]
}
```

## 4. C++ Library API

For developers using the system as a C++ library, the public API is defined in the `include/llm/llm.h` header file.

### `llm::Engine`

The main class for interacting with the LLM.

#### `Engine(const std::string& model_path)`

Constructs a new `Engine` instance, loading the model from the specified path.

#### `std::string generate(const std::string& prompt, int max_tokens)`

Generates text based on the given prompt.

---

*End of Document*
