#pragma once

#include <string>
#include <map>

class MetricsExporter {
public:
    MetricsExporter();
    void incrementCounter(const std::string& name, const std::map<std::string, std::string>& labels = {});
    void setGauge(const std::string& name, double value, const std::map<std::string, std::string>& labels = {});
    std::string exposePrometheus() const;

private:
    std::map<std::string, double> counters;
    std::map<std::string, double> gauges;
};
