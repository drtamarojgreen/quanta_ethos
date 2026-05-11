#pragma once

#include <string>

class UserDataApi {
public:
    UserDataApi();
    std::string exportData(const std::string& user_id);
    bool deleteData(const std::string& user_id);
};
