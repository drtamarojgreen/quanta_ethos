#include "numa_strategy.h"
#include <iostream>

NumaStrategy::NumaStrategy() {}

void NumaStrategy::pinToNode(int id) {
    std::cout << "Pinning thread to NUMA node " << id << "...\n";
}

int NumaStrategy::getCurrentNode() const {
    return 0;
}
