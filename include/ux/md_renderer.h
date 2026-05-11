#pragma once

#include <string>

class MdRenderer {
public:
    MdRenderer();
    std::string render(const std::string& markdown);
};
