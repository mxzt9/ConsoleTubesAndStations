#include "eventsHandler.h"

#include <conio.h>

Key EventsHandler::listenForKey() {
    int symbol = _getch();

    if (symbol == 0 || symbol == 224) {
        int extendedSymbol = _getch();

        switch (extendedSymbol) {
            case 72:
                return Key::Up;
            case 80:
                return Key::Down;
            default:
                return Key::Unknown;
        }
    }

    switch (symbol) {
        case 13:
            return Key::Enter;
        case 27:
            return Key::Escape;
        default:
            return Key::Unknown;
    }
}
