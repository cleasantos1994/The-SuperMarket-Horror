#include "SaveLoad.h"
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

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
    
    std::string data = j.dump(4);
    SDL_RWops* rw = SDL_RWFromFile("save.json", "w");
    if (rw) {
        SDL_RWwrite(rw, data.c_str(), 1, data.size());
        SDL_RWclose(rw);
    }
}

bool SaveLoad::LoadGame(GameStateData& state) {
    SDL_RWops* rw = SDL_RWFromFile("save.json", "r");
    if (!rw) return false;
    
    try {
        Sint64 size = SDL_RWsize(rw);
        std::vector<char> buffer(size + 1);
        SDL_RWread(rw, buffer.data(), 1, size);
        SDL_RWclose(rw);
        buffer[size] = '\0';

        json j = json::parse(buffer.data());
        state.day1TasksDone = j.value("day1TasksDone", 0);
        state.day5TasksDone = j.value("day5TasksDone", 0);
        state.hasCarKeys = j.value("hasCarKeys", false);
        state.fearLevel = j.value("fearLevel", 0);
        state.playTime = j.value("playTime", 0.0f);
        state.deathCount = j.value("deathCount", 0);
        
        int sceneIdx = j.value("currentScene", 1);
        if (sceneIdx >= 0 && sceneIdx <= 15) {
            state.currentScene = static_cast<GameScene>(sceneIdx);
        }
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
    
    std::string data = j.dump(4);
    SDL_RWops* rw = SDL_RWFromFile("settings.json", "w");
    if (rw) {
        SDL_RWwrite(rw, data.c_str(), 1, data.size());
        SDL_RWclose(rw);
    }
}

void SaveLoad::LoadSettings(GameStateData& state) {
    SDL_RWops* rw = SDL_RWFromFile("settings.json", "r");
    if (!rw) return;
    
    try {
        Sint64 size = SDL_RWsize(rw);
        std::vector<char> buffer(size + 1);
        SDL_RWread(rw, buffer.data(), 1, size);
        SDL_RWclose(rw);
        buffer[size] = '\0';

        json j = json::parse(buffer.data());
        state.masterVolume = j.value("masterVolume", 0.8f);
        state.mouseSensitivity = j.value("mouseSensitivity", 0.1f);
        state.fullscreen = j.value("fullscreen", false);
        state.vSync = j.value("vSync", true);
        state.showSubtitles = j.value("showSubtitles", true);
    } catch (...) {}
}

bool SaveLoad::SaveExists() {
    SDL_RWops* rw = SDL_RWFromFile("save.json", "r");
    if (rw) {
        SDL_RWclose(rw);
        return true;
    }
    return false;
}
