#include <iostream>
#include <string>

#include "core_engine.h"

int main(int argc, char* argv[]) {
    std::string prompt = "Hello from PrismQuanta";
    if (argc > 1) {
        prompt = argv[1];
    }

    CoreEngine core_engine;
    std::cout << core_engine.generate_response(prompt) << std::endl;
    return 0;
}
