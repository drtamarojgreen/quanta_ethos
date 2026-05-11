#include "etl_worker.h"
#include <iostream>

EtlWorker::EtlWorker(const std::string& warehouse_url) : url(warehouse_url) {}

bool EtlWorker::extract(const std::string& source) {
    std::cout << "Extracting data from " << source << "...\n";
    return true;
}

bool EtlWorker::transform() {
    std::cout << "Transforming data for BigQuery format...\n";
    return true;
}

bool EtlWorker::load() {
    std::cout << "Loading data into " << url << "...\n";
    return true;
}
