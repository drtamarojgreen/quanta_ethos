#include <restinio/all.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include "core_engine.h"

using json = nlohmann::json;

int main() {
    CoreEngine core_engine;

    try {
        restinio::run(
            restinio::on_this_thread<>()
                .port(8080)
                .address("localhost")
                .request_handler([&core_engine](auto req) {
                    if (req->header().request_target() == "/ping") {
                        return req->create_response()
                            .set_header(restinio::http_field::content_type, "application/json")
                            .set_body(R"({"status": "ok"})")
                            .done();
                    }
                    if (req->header().request_target() == "/v1/generate" && req->header().method() == restinio::http_method_post()) {
                        try {
                            json body = json::parse(req->body());
                            std::string prompt = body.at("prompt");
                            std::string response = core_engine.generate_response(prompt);
                            json response_json = {{"response", response}};
                            return req->create_response()
                                .set_header(restinio::http_field::content_type, "application/json")
                                .set_body(response_json.dump())
                                .done();
                        } catch (const json::exception& e) {
                            return req->create_response(restinio::status_bad_request())
                                .set_body("Invalid JSON: " + std::string(e.what()))
                                .done();
                        }
                    }
                    return req->create_response(restinio::status_not_found()).done();
                }));
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}