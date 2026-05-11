#pragma once

#include <string>

class NumaStrategy {
public:
    NumaStrategy();
    void pinToNode(int node_id);
    int getCurrentNode() const;
};
