#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    std::string input(reinterpret_cast<const char*>(Data), Size);
    // Fuzz test target
    return 0;
}
