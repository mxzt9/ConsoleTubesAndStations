#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

void initTubeCSV(const std::filesystem::path& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: cant open file for writing: " << filename << std::endl;
        return;
    }

    file << "Id;Name;Length;Diameter;isInRepair\n";
}

void initComperessStationCSV(const std::filesystem::path& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: cant open file for writing: " << filename << std::endl;
        return;
    }
    file << "Id;Name;NumOfWorkshop;NumOfActiveWorkshop;Type\n";
}

void initCSV(const std::filesystem::path& tubeFilename, const std::filesystem::path& compressFilename) {
    std::filesystem::path dataPath = std::filesystem::current_path() / "data";

    if (!std::filesystem::exists(dataPath)) {
        std::filesystem::create_directories(dataPath);
    }

    std::filesystem::path tubePath = dataPath / tubeFilename;
    std::filesystem::path csPath = dataPath / compressFilename;

    if (!std::filesystem::exists(tubePath)) {
        initTubeCSV(tubePath);
    }
    if (!std::filesystem::exists(csPath)) {
        initComperessStationCSV(csPath);
    }
}

struct Tube {
    std::string name;
    int length;
    int d;
    bool isInRepair;
};

struct CompressStation {
    std::string name;
    int numOfWorkshop;
    int numOfActiveWorkshop;

    enum class CompressStationType {
        Head,
        Linear,
        Boost
    };

    CompressStationType type;
};

class Terminal {
private:
    std::filesystem::path tubeFilePath;
    std::filesystem::path csFilePath;

    enum class Menu {
        AddTube,
        AddCompressStation,
        EditTube,
        EditCompressStation,
        LookUpAllObjects,
        Save,
        Load
    };

public:
    Terminal(std::filesystem::path tubePath, std::filesystem::path csPath)
        : tubeFilePath(std::move(tubePath)), csFilePath(std::move(csPath)) {}
};

int main() {
    initCSV("tubes.csv", "comperess_station.csv");
    return 0;
}