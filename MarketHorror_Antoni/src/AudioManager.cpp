#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "AudioManager.h"
#include <iostream>

static ma_engine engine;
static bool initialized = false;
static ma_sound currentBGM;

bool AudioManager::Init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cerr << "Failed to initialize miniaudio engine." << std::endl;
        return false;
    }
    initialized = true;
    return true;
}

void AudioManager::Shutdown() {
    if (initialized) {
        ma_engine_uninit(&engine);
        initialized = false;
    }
}

void AudioManager::PlayBGM(const std::string& path, bool loop, float volume) {
    if (!initialized) return;
    StopBGM();
    ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &currentBGM);
    ma_sound_set_looping(&currentBGM, loop ? MA_TRUE : MA_FALSE);
    ma_sound_set_volume(&currentBGM, volume);
    ma_sound_start(&currentBGM);
}

void AudioManager::StopBGM() {
    if (!initialized) return;
    if (ma_sound_is_playing(&currentBGM)) {
        ma_sound_stop(&currentBGM);
        ma_sound_uninit(&currentBGM);
    }
}

void AudioManager::PlaySFX(const std::string& path, float volume) {
    if (!initialized) return;
    // Basic play-and-forget for SFX
    ma_engine_play_sound(&engine, path.c_str(), NULL);
}