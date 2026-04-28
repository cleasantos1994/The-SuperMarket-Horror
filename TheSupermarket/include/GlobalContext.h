#pragma once
#include "GameState.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "UIRenderer.h"
#include "Camera.h"
#include "MarketMap.h"
#include "AntoniAI.h"
#include "TaskSystem.h"
#include "EventManager.h"
#include "ParticleSystem.h"
#include "PostProcess.h"

// The GlobalContext acts as the "Internal" hub of the game.
// It centralizes all manager pointers for easy access and internal hooking.
class GlobalContext {
public:
    static GlobalContext& Get() {
        static GlobalContext instance;
        return instance;
    }

    GameStateMachine* gsm      = nullptr;
    AudioManager*     audio    = nullptr;
    InputManager*     input    = nullptr;
    UIRenderer*       ui       = nullptr;
    Camera*           camera   = nullptr;
    MarketMap*        market   = nullptr;
    AntoniAI*         antoni   = nullptr;
    TaskSystem*       tasks    = nullptr;
    EventManager*     events   = nullptr;
    ParticleSystem*   particles= nullptr;
    PostProcess*      post     = nullptr;

    void Init() {
        gsm = &GameStateMachine::Get();
        audio = &AudioManager::Get();
        input = &InputManager::Get();
    }

private:
    GlobalContext() = default;
};
