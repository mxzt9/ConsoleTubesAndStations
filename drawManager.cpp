#include "drawManager.h"

#include <cstdlib>
#include <iostream>
#include <windows.h>

DrawManager::DrawManager() {
    getTeminalSize();
    initRect();
}

void DrawManager::goToXY(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, position);
}

void DrawManager::clearTerminal() {
    std::system("cls");
}

void DrawManager::initRect() {
    menuRect = { terminalWidth / 4, 0, terminalWidth / 2, terminalHeigth * 3 / 5 };
}

void DrawManager::getTeminalSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    terminalWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    terminalHeigth = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void DrawManager::drawBorder(const Rect& borderShape, const std::string& borderTitle) {
    const auto& [startBorderX, startBorderY, borderWidth, borderHeight] = borderShape;
    const int endBorderX = startBorderX + borderWidth;
    const int endBorderY = startBorderY + borderHeight;

    for (int x = startBorderX; x <= endBorderX; ++x) {
        goToXY(x, startBorderY);
        std::cout << (x == startBorderX || x == endBorderX ? '+' : '-');

        goToXY(x, endBorderY);
        std::cout << (x == startBorderX || x == endBorderX ? '+' : '-');
    }

    for (int y = startBorderY + 1; y <= endBorderY - 1; ++y) {
        goToXY(startBorderX, y);
        std::cout << '|';

        goToXY(endBorderX, y);
        std::cout << '|';
    }

    goToXY(startBorderX + 3, startBorderY);
    std::cout << borderTitle;
}

void DrawManager::drawSelectableList(
    const std::string& title,
    const std::vector<std::string>& items,
    int cursorIndex,
    const std::string& footer
) {
    drawBorder(menuRect, " " + title + " ");

    const auto& [startBorderX, startBorderY, borderWidth, borderHeigth] = menuRect;

    int startMenuX = startBorderX + borderWidth / 4;
    int startMenuY = startBorderY + borderHeigth / 4;

    for (std::size_t i = 0; i < items.size(); ++i) {
        goToXY(startMenuX, startMenuY + static_cast<int>(i));
        std::cout << (static_cast<int>(i) == cursorIndex ? "> " : "  ");
        std::cout << items[i];
    }

    if (!footer.empty()) {
        goToXY(startMenuX, startMenuY + static_cast<int>(items.size()) + 2);
        std::cout << footer;
    }
}
