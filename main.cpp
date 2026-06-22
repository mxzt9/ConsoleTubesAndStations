#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>

#include "eventsHandler.h"
#include "drawManager.h"
#include "fileUtils.h"

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

class App;

class Screen {
public:
    virtual ~Screen() = default;

    virtual void draw(DrawManager& drawManager) = 0;
    virtual void handleInput(Key key, App& app) = 0;
};

enum class TubeLoadFilter {
    ByName,
    OnlyInRepair,
    All
};

enum class CompressStationLoadFilter {
    ByName,
    UsageAscending,
    UsageDescending,
    All
};

struct LoadCsvState {
    bool loadTubes = false;
    bool loadCompressStations = false;
    TubeLoadFilter tubeFilter = TubeLoadFilter::All;
    CompressStationLoadFilter compressStationFilter = CompressStationLoadFilter::All;
};

class App {
    private:
        std::filesystem::path tubeFilename = "tubes.csv";
        std::filesystem::path compressFilename = "comperess_station.csv";

        std::filesystem::path tubePath;
        std::filesystem::path compressPath;

        bool isRunning = true;

        DrawManager drawManager;
        EventsHandler eventsHandler;
        std::unique_ptr<Screen> currentScreen;
        LoadCsvState loadCsvState;

    public:
        void initApp();
        void run();
        void stop();

        void openMainMenu();
        void openLoadObjectType();
        void openLoadFilters();
        void openTubeLoadFilter();
        void openCompressStationLoadFilter();
        void finishLoadCsv();

        LoadCsvState& getLoadCsvState();
};

static void moveCursor(Key key, int& cursorIndex, std::size_t itemsCount) {
    if (itemsCount == 0) {
        cursorIndex = 0;
        return;
    }

    if (key == Key::Up) {
        cursorIndex = cursorIndex == 0 ? static_cast<int>(itemsCount - 1) : cursorIndex - 1;
    }

    if (key == Key::Down) {
        cursorIndex = (cursorIndex + 1) % static_cast<int>(itemsCount);
    }
}

class MainMenuScreen : public Screen {
private:
    std::vector<std::string> items {
        "Add tube",
        "Add compressor station",
        "Edit tube",
        "Edit compressor station",
        "Show objects",
        "Save",
        "Load CSV"
    };
    int cursorIndex = 0;

public:
    void draw(DrawManager& drawManager) override {
        drawManager.drawSelectableList(
            "OilPanel",
            items,
            cursorIndex,
            "Arrows - move, Enter - choose, Esc - exit"
        );
    }

    void handleInput(Key key, App& app) override {
        moveCursor(key, cursorIndex, items.size());

        if (key == Key::Escape) {
            app.stop();
            return;
        }

        if (key != Key::Enter) {
            return;
        }

        switch (cursorIndex) {
            case 6:
                app.openLoadObjectType();
                break;
            case 7:
                app.stop();
                break;
            default:
                break;
        }
    }
};

class LoadObjectTypeScreen : public Screen {
private:
    std::vector<std::string> items {
        "Tubes",
        "CompressStations",
        "All"
    };
    int cursorIndex = 0;

public:
    explicit LoadObjectTypeScreen(const LoadCsvState& state) {
        if (state.loadTubes && state.loadCompressStations) {
            cursorIndex = 2;
        } else if (state.loadCompressStations) {
            cursorIndex = 1;
        } else {
            cursorIndex = 0;
        }
    }

    void draw(DrawManager& drawManager) override {
        drawManager.drawSelectableList(
            "Load CSV: objects",
            items,
            cursorIndex,
            "Arrows - move, Enter - choose, Esc - back"
        );
    }

    void handleInput(Key key, App& app) override {
        moveCursor(key, cursorIndex, items.size());

        if (key == Key::Escape) {
            app.openMainMenu();
            return;
        }

        if (key != Key::Enter) {
            return;
        }

        LoadCsvState& state = app.getLoadCsvState();
        state.loadTubes = cursorIndex == 0 || cursorIndex == 2;
        state.loadCompressStations = cursorIndex == 1 || cursorIndex == 2;
        state.tubeFilter = TubeLoadFilter::All;
        state.compressStationFilter = CompressStationLoadFilter::All;

        if (cursorIndex == 2) {
            app.finishLoadCsv();
            return;
        }

        app.openLoadFilters();
    }
};

class TubeLoadFilterScreen : public Screen {
private:
    std::vector<std::string> items {
        "Select by name",
        "Select only in repair",
        "Select all"
    };
    int cursorIndex = 0;

public:
    explicit TubeLoadFilterScreen(const LoadCsvState& state) {
        switch (state.tubeFilter) {
            case TubeLoadFilter::ByName:
                cursorIndex = 0;
                break;
            case TubeLoadFilter::OnlyInRepair:
                cursorIndex = 1;
                break;
            case TubeLoadFilter::All:
                cursorIndex = 2;
                break;
        }
    }

    void draw(DrawManager& drawManager) override {
        drawManager.drawSelectableList(
            "Load CSV: tube filter",
            items,
            cursorIndex,
            "Arrows - move, Enter - load, Esc - back"
        );
    }

    void handleInput(Key key, App& app) override {
        moveCursor(key, cursorIndex, items.size());

        if (key == Key::Escape) {
            app.openLoadObjectType();
            return;
        }

        if (key != Key::Enter) {
            return;
        }

        LoadCsvState& state = app.getLoadCsvState();

        switch (cursorIndex) {
            case 0:
                state.tubeFilter = TubeLoadFilter::ByName;
                break;
            case 1:
                state.tubeFilter = TubeLoadFilter::OnlyInRepair;
                break;
            case 2:
                state.tubeFilter = TubeLoadFilter::All;
                break;
        }

        app.finishLoadCsv();
    }
};

class CompressStationLoadFilterScreen : public Screen {
private:
    std::vector<std::string> items {
        "Select by name",
        "Select by percent of usage (ascending)",
        "Select by percent of usage (descending)",
        "Select all"
    };
    int cursorIndex = 0;

public:
    explicit CompressStationLoadFilterScreen(const LoadCsvState& state) {
        switch (state.compressStationFilter) {
            case CompressStationLoadFilter::ByName:
                cursorIndex = 0;
                break;
            case CompressStationLoadFilter::UsageAscending:
                cursorIndex = 1;
                break;
            case CompressStationLoadFilter::UsageDescending:
                cursorIndex = 2;
                break;
            case CompressStationLoadFilter::All:
                cursorIndex = 3;
                break;
        }
    }

    void draw(DrawManager& drawManager) override {
        drawManager.drawSelectableList(
            "Load CSV: CS filter",
            items,
            cursorIndex,
            "Arrows - move, Enter - load, Esc - back"
        );
    }

    void handleInput(Key key, App& app) override {
        moveCursor(key, cursorIndex, items.size());

        if (key == Key::Escape) {
            app.openLoadObjectType();
            return;
        }

        if (key != Key::Enter) {
            return;
        }

        LoadCsvState& state = app.getLoadCsvState();

        switch (cursorIndex) {
            case 0:
                state.compressStationFilter = CompressStationLoadFilter::ByName;
                break;
            case 1:
                state.compressStationFilter = CompressStationLoadFilter::UsageAscending;
                break;
            case 2:
                state.compressStationFilter = CompressStationLoadFilter::UsageDescending;
                break;
            case 3:
                state.compressStationFilter = CompressStationLoadFilter::All;
                break;
        }

        app.finishLoadCsv();
    }
};

void App::initApp() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::filesystem::path dataPath = std::filesystem::current_path() / "data";

    if (!std::filesystem::exists(dataPath)) {
        std::filesystem::create_directories(dataPath);
    }

    tubePath = dataPath / tubeFilename;
    compressPath = dataPath / compressFilename;

    initCSV(tubePath, compressPath);
    openMainMenu();
    run();
}

void App::run() {
    while (isRunning) {
        drawManager.clearTerminal();

        if (currentScreen) {
            currentScreen->draw(drawManager);
            Key key = eventsHandler.listenForKey();
            currentScreen->handleInput(key, *this);
        }
    }
}

void App::stop() {
    isRunning = false;
}

void App::openMainMenu() {
    currentScreen = std::make_unique<MainMenuScreen>();
}

void App::openLoadObjectType() {
    currentScreen = std::make_unique<LoadObjectTypeScreen>(loadCsvState);
}

void App::openLoadFilters() {
    if (loadCsvState.loadTubes) {
        openTubeLoadFilter();
        return;
    }

    if (loadCsvState.loadCompressStations) {
        openCompressStationLoadFilter();
        return;
    }

    openLoadObjectType();
}

void App::openTubeLoadFilter() {
    currentScreen = std::make_unique<TubeLoadFilterScreen>(loadCsvState);
}

void App::openCompressStationLoadFilter() {
    currentScreen = std::make_unique<CompressStationLoadFilterScreen>(loadCsvState);
}

void App::finishLoadCsv() {
    openMainMenu();
}

LoadCsvState& App::getLoadCsvState() {
    return loadCsvState;
}

int main() {
    App app;
    app.initApp();

    return 0;
}
