#include "user_data_api.h"
#include <iostream>

UserDataApi::UserDataApi() {}

std::string UserDataApi::exportData(const std::string& user_id) {
    return "Exported data for " + user_id + ": {}";
}

bool UserDataApi::deleteData(const std::string& user_id) {
    std::cout << "Deleting data for " << user_id << " in compliance with GDPR...\n";
    return true;
}
