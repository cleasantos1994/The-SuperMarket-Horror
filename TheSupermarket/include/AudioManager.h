#pragma once
#include <string>
#include <map>

class AudioManager {
public:
    static constexpr const char* BGM_MENU = "assets/audio/bgm_menu.wav";
    static constexpr const char* BGM_NORMAL_SHIFT = "assets/audio/bgm_normal_shift.wav";
    static constexpr const char* BGM_CHASE = "assets/audio/bgm_chase.wav";
    static constexpr const char* BGM_TENSION = "assets/audio/bgm_tension.wav";

    static constexpr const char* SFX_CLICK = "assets/audio/sfx_ui_click.wav";
    static constexpr const char* SFX_BEEP = "assets/audio/sfx_beep.wav";

    static AudioManager& Get() {
        static AudioManager instance;
        return instance;
    }

    bool Init();
    void Shutdown();

    void PlayBGM(const std::string& path, bool loop = true, float volume = 0.5f);
    void StopBGM();
    void PlaySFX(const std::string& path, float volume = 1.0f);

private:
    AudioManager() = default;
    // Implementation details will be in AudioManager.cpp using miniaudio
};
