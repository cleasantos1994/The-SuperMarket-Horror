#pragma once
#include "GameState.h"
#include "Player.h"

class SaveLoad {
public:
    static void SaveGame(const GameStateData& state, const Player& player);
    static bool LoadGame(GameStateData& state, Player& player);
    static void SaveSettings(const GameStateData& state);
    static void LoadSettings(GameStateData& state);
    static bool SaveExists();
};
