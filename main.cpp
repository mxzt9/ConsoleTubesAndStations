#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <windows.h>
#include "file_utils.h"

void goToXY(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, position);
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
    int width, height;

    std::filesystem::path tubePath;
    std::filesystem::path compressPath;

    std::vector<Tube> tubeVector;
    std::vector<CompressStation> compressVector;

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
    Terminal(std::filesystem::path tubePath, std::filesystem::path compressPath): tubePath(std::move(tubePath)), compressPath(std::move(compressPath)) {
        getTeminalSize();
    }

    void getTeminalSize() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    };

    void drawBorder() {
        int startX = width / 4;
        int endX = startX + width / 2;

        int startY = 0;
        int endY = startY + height / 5 * 3;

        const std::string topLeft = "┏";
        const std::string topRight = "┓";
        const std::string bottomLeft = "┗";
        const std::string bottomRight = "┛";
        const std::string horizontal = "━";
        const std::string vertical = "┃";

        for (int x = startX; x <= endX; ++x) {

            goToXY(x, startY);
            if (x == startX) {
                std::cout << topLeft;
            } else if (x == endX) {
                std::cout << topRight;
            } else {
                std::cout << horizontal;
            }

            goToXY(x, endY);
            if (x == startX) {
                std::cout << bottomLeft;
            } else if (x == endX) {
                std::cout << bottomRight;
            } else {
                std::cout << horizontal;
            }
        }

        for (int y = startY + 1; y <= endY - 1; ++y) {

            goToXY(startX, y);
            std::cout << vertical;

            goToXY(endX, y);
            std::cout << vertical;

        }

        goToXY(startX + 3, startY);
        std::cout << " OilPanel ";
    };
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::filesystem::path dataPath = std::filesystem::current_path() / "data";

    if (!std::filesystem::exists(dataPath)) {
        std::filesystem::create_directories(dataPath);
    }

    std::filesystem::path tubeFilename = "tubes.csv";
    std::filesystem::path compressFilename = "comperess_station.csv";

    std::filesystem::path tubePath = dataPath / tubeFilename;
    std::filesystem::path compressPath = dataPath / compressFilename;


    initCSV(tubePath, compressPath);
    Terminal MainTerminal(tubePath, compressPath);
    MainTerminal.drawBorder();

    char t;
    std::cin >> t;

    return 0;
}
