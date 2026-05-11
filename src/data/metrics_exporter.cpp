#include "metrics_exporter.h"
#include <sstream>

MetricsExporter::MetricsExporter() {}

void MetricsExporter::incrementCounter(const std::string& name, const std::map<std::string, std::string>& labels) {
    counters[name]++;
}

void MetricsExporter::setGauge(const std::string& name, double value, const std::map<std::string, std::string>& labels) {
    gauges[name] = value;
}

std::string MetricsExporter::exposePrometheus() const {
    std::ostringstream oss;
    for (const auto& c : counters) {
        oss << c.first << "_total " << c.second << "\n";
    }
    for (const auto& g : gauges) {
        oss << g.first << " " << g.second << "\n";
    }
    return oss.str();
}
