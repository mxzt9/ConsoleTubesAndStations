#include "fileUtils.h"

#include <iostream>
#include <fstream>

void initCSV(const std::filesystem::path& tubePath, const std::filesystem::path& compressPath) {

    if (!std::filesystem::exists(tubePath)) {
        std::ofstream file(tubePath);

        if (!file.is_open()) {
            std::cerr << "Error: cant open file for writing: " << tubePath << std::endl;
            return;
        }

        file << "Id;Name;Length;Diameter;isInRepair\n";
    
    }

    if (!std::filesystem::exists(compressPath)) {
        std::ofstream file(compressPath);

        if (!file.is_open()) {
            std::cerr << "Error: cant open file for writing: " << compressPath << std::endl;
            return;
        }

        file << "Id;Name;NumOfWorkshop;NumOfActiveWorkshop;Type\n";
    }
};


void writeCSV(const std::filesystem::path& filePath, const std::vector<std::string>& lines) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "Error: No such file " << filePath << std::endl;
        return;
    }

    std::ofstream file(filePath);
    if (!file.is_open()) {
            std::cerr << "Error: cant open file for writing: " << filePath << std::endl;
            return;
        }
    
    for (const std::string& currentLine: lines) {
        file << currentLine + '\n';
    }
};


std::vector<std::string> readCSV(const std::filesystem::path& filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "Error: No such file " << filePath << std::endl;
        return {};
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: cant open file for writing: " << filePath << std::endl;
        return {};
    }

    std::vector<std::string> lines;

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
};