#pragma once

enum class Key {
    Up,
    Down,
    Enter,
    Escape,
    Unknown
};


class EventsHandler {
    public:
        EventsHandler() = default;
        Key listenForKey();
};
