#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>

class DataExporter {
public:
    static void exportToMarkdown(const std::string& path, const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& rows) {
        std::ofstream file(path);
        if (!file.is_open()) return;

        file << "| ";
        for (const auto& h : headers) file << h << " | ";
        file << "\n| ";
        for (size_t i = 0; i < headers.size(); ++i) file << "--- | ";
        file << "\n";

        for (const auto& row : rows) {
            file << "| ";
            for (const auto& cell : row) file << cell << " | ";
            file << "\n";
        }
    }

    static void exportToCSV(const std::string& path, const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& rows) {
        std::ofstream file(path);
        if (!file.is_open()) return;

        for (size_t i = 0; i < headers.size(); ++i) {
            file << headers[i] << (i == headers.size() - 1 ? "" : ",");
        }
        file << "\n";

        for (const auto& row : rows) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i] << (i == row.size() - 1 ? "" : ",");
            }
            file << "\n";
        }
    }

    static void exportToJSON(const std::string& path, const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& rows) {
        std::ofstream file(path);
        if (!file.is_open()) return;

        file << "[\n";
        for (size_t r = 0; r < rows.size(); ++r) {
            file << "  {\n";
            for (size_t c = 0; c < headers.size(); ++c) {
                file << "    \"" << headers[c] << "\": \"" << (c < rows[r].size() ? rows[r][c] : "") << "\"" << (c == headers.size() - 1 ? "" : ",") << "\n";
            }
            file << "  }" << (r == rows.size() - 1 ? "" : ",") << "\n";
        }
        file << "]\n";
    }
};
