#include "ux/data_exporter.h"
#include "test_framework.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE(DataExporter_MarkdownExport) {
    std::string path = "test/temp/test_export.md";
    fs::create_directories("test/temp");

    std::vector<std::string> headers = {"H1", "H2"};
    std::vector<std::vector<std::string>> rows = {{"R1C1", "R1C2"}};

    DataExporter::exportToMarkdown(path, headers, rows);

    ASSERT_TRUE(fs::exists(path));
    std::ifstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "| H1 | H2 | ");

    fs::remove_all("test/temp");
}

TEST_CASE(DataExporter_CSVExport) {
    std::string path = "test/temp/test_export.csv";
    fs::create_directories("test/temp");

    std::vector<std::string> headers = {"H1", "H2"};
    std::vector<std::vector<std::string>> rows = {{"R1C1", "R1C2"}};

    DataExporter::exportToCSV(path, headers, rows);

    ASSERT_TRUE(fs::exists(path));
    std::ifstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "H1,H2");

    fs::remove_all("test/temp");
}
