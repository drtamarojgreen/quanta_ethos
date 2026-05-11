#pragma once

#include <string>
#include <vector>

class GraphConnector {
public:
    GraphConnector(const std::string& endpoint);
    std::vector<std::string> query(const std::string& cypher);
    void connect();

private:
    std::string endpoint;
};
