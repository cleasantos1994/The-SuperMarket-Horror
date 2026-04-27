#pragma once
#include <string>

enum class GameScene {
    LOADING,
    MAIN_MENU,
    SETTINGS,
    CUTSCENE_DAY1,
    GAMEPLAY_DAY1,
    CUTSCENE_POOP1,
    CUTSCENE_DAY5,
    GAMEPLAY_DAY5,
    CUTSCENE_POOP2,
    CUTSCENE_CHASE,
    GAMEPLAY_CHASE,
    PAUSE_MENU,
    JUMPSCARE,
    GAME_OVER,
    WIN_SCREEN,
    CREDITS
};

enum class Day1Task {
    RESTOCK_SHELF_A,
    RESTOCK_SHELF_B,
    RESTOCK_SHELF_C,
    GREET_CUSTOMER_1,
    GREET_CUSTOMER_2,
    DONE
};

enum class Day5Task {
    RESTOCK_SHELF_CEREAL,
    HELP_CUSTOMER_FIND_ITEM,
    SCAN_ITEMS_AT_REGISTER,
    DONE
};

struct GameStateData {
    GameScene  currentScene    = GameScene::LOADING;
    GameScene  previousScene   = GameScene::LOADING;
    float      loadProgress    = 0.0f;

    // Work progress
    Day1Task   day1Task        = Day1Task::RESTOCK_SHELF_A;
    Day5Task   day5Task        = Day5Task::RESTOCK_SHELF_CEREAL;
    int        day1TasksDone   = 0;
    int        day5TasksDone   = 0;
    bool       hasCarKeys      = false;
    bool       antoniFocusedOnPlayer = false;

    // Fear / chase state
    int        fearLevel       = 0;
    bool       antoniChasing   = false;
    bool       isPaused        = false;
    float      playTime        = 0.0f;
    int        deathCount      = 0;
    bool       isSlipping      = false;
    float      slipTimer       = 0.0f;

    // Settings
    float      masterVolume    = 0.8f;
    float      mouseSensitivity= 0.1f;
    bool       fullscreen      = false;
    bool       vSync           = true;
    bool       showSubtitles   = true;

    // Sub-Protagonists presence
    bool       calebAlive      = true;
    bool       williamAlive    = true;
    bool       guilhermeAlive  = true;
};

class GameStateMachine {
public:
    static GameStateMachine& Get() {
        static GameStateMachine instance;
        return instance;
    }
    void TransitionTo(GameScene scene) {
        state_.previousScene = state_.currentScene;
        state_.currentScene = scene;
    }
    GameScene Current() const { return state_.currentScene; }
    GameStateData& Data() { return state_; }
    void Reset() { state_ = GameStateData(); }
private:
    GameStateData state_;
    GameStateMachine() = default;
};
