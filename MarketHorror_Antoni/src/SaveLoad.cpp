#include "SaveLoad.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void SaveLoad::SaveGame(const GameStateData& state) {
    json j;
    j["day1TasksDone"] = state.day1TasksDone;
    j["day5TasksDone"] = state.day5TasksDone;
    j["hasCarKeys"] = state.hasCarKeys;
    j["fearLevel"] = state.fearLevel;
    j["playTime"] = state.playTime;
    j["deathCount"] = state.deathCount;
    j["currentScene"] = static_cast<int>(state.currentScene);
    
    std::ofstream file("assets/data/save.json");
    if (file.is_open()) {
        file << j.dump(4);
    }
}

bool SaveLoad::LoadGame(GameStateData& state) {
    std::ifstream file("assets/data/save.json");
    if (!file.is_open()) return false;
    
    try {
        json j;
        file >> j;
        state.day1TasksDone = j.value("day1TasksDone", 0);
        state.day5TasksDone = j.value("day5TasksDone", 0);
        state.hasCarKeys = j.value("hasCarKeys", false);
        state.fearLevel = j.value("fearLevel", 0);
        state.playTime = j.value("playTime", 0.0f);
        state.deathCount = j.value("deathCount", 0);
        state.currentScene = static_cast<GameScene>(j.value("currentScene", 1));
        return true;
    } catch (...) {
        return false;
    }
}

void SaveLoad::SaveSettings(const GameStateData& state) {
    json j;
    j["masterVolume"] = state.masterVolume;
    j["mouseSensitivity"] = state.mouseSensitivity;
    j["fullscreen"] = state.fullscreen;
    j["vSync"] = state.vSync;
    j["showSubtitles"] = state.showSubtitles;
    
    std::ofstream file("assets/data/settings.json");
    if (file.is_open()) {
        file << j.dump(4);
    }
}

void SaveLoad::LoadSettings(GameStateData& state) {
    std::ifstream file("assets/data/settings.json");
    if (!file.is_open()) return;
    
    try {
        json j;
        file >> j;
        state.masterVolume = j.value("masterVolume", 0.8f);
        state.mouseSensitivity = j.value("mouseSensitivity", 0.1f);
        state.fullscreen = j.value("fullscreen", false);
        state.vSync = j.value("vSync", true);
        state.showSubtitles = j.value("showSubtitles", true);
    } catch (...) {}
}

bool SaveLoad::SaveExists() {
    std::ifstream file("assets/data/save.json");
    return file.good();
}