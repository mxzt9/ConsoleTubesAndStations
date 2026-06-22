#pragma once

#include <string>
#include <filesystem>
#include <vector>


void initCSV(const std::filesystem::path& tubePath, const std::filesystem::path& compressPath);
void writeCSV(const std::filesystem::path& filePath, const std::vector<std::string>&);
std::vector<std::string> readCSV(const std::filesystem::path& filePath);

