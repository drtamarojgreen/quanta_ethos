#include "openapi_gen.h"

OpenApiGen::OpenApiGen() {}

std::string OpenApiGen::generateSpec() {
    return R"({
  "openapi": "3.0.0",
  "info": {
    "title": "PrismQuanta API",
    "version": "1.0.0"
  },
  "paths": {
    "/v1/generate": {
      "post": {
        "summary": "Generate AI response"
      }
    }
  }
})";
}
