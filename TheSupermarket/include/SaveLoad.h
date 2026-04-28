#pragma once
#include "GameState.h"
#include "Player.h"

#include "Export.h"

class GAME_API SaveLoad {
public:
    static void SaveGame(const GameStateData& state, const Player& player);
    static bool LoadGame(GameStateData& state, Player& player);
    static void SaveSettings(const GameStateData& state);
    static void LoadSettings(GameStateData& state);
    static bool SaveExists();
};
