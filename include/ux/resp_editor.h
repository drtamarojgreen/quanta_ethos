#pragma once

#include <string>

class RespEditor {
public:
    RespEditor();
    std::string edit(const std::string& original, const std::string& new_text);
};
