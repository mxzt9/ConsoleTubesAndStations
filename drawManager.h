#pragma once

#include <string>
#include <vector>

struct Rect {
    int x, y, w, h;
};

class DrawManager {  
private:
    int terminalWidth = 0;
    int terminalHeigth = 0;

    Rect menuRect {0, 0, 0, 0};

    void goToXY(int x, int y);
    void drawBorder(const Rect& borderShape, const std::string& borderTitle);
    
    void initRect();

    void getTeminalSize();

public:
    DrawManager();

    void clearTerminal();

    void drawSelectableList(
        const std::string& title,
        const std::vector<std::string>& items,
        int cursorIndex,
        const std::string& footer = ""
    );
};
