#pragma once
#include "GameState.h"

class SaveLoad {
public:
    static void SaveGame(const GameStateData& state);
    static bool LoadGame(GameStateData& state);
    static void SaveSettings(const GameStateData& state);
    static void LoadSettings(GameStateData& state);
    static bool SaveExists();
};
