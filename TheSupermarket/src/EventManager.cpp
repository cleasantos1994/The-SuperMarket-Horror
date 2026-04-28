#include "EventManager.h"
#include "AntoniAI.h"
#include "CustomerAI.h"
#include "MarketMap.h"
#include "AudioManager.h"
#include "GameState.h"
#include "Camera.h"
#include "InputManager.h"
#include "GlobalContext.h"
#include "Player.h"

void EventManager::RegisterEvent(StoryEvent event) {
    events_.push_back(event);
}

void EventManager::Update(float dt) {
    for (auto& e : events_) {
        if (!e.fired && e.trigger()) {
            e.action();
            e.fired = true;
            if (e.blocking) {
                blocking_ = true;
                blockTimer_ = 3.0f; 
            }
        }
    }
    if (blocking_) {
        blockTimer_ -= dt;
        if (blockTimer_ <= 0.0f) blocking_ = false;
    }
}

bool EventManager::IsBlocking() const { return blocking_; }
void EventManager::SkipCurrent() { blocking_ = false; }

void EventManager::RegisterDay1Events(AntoniAI& a, CustomerAI& c, AudioManager& audio, GameStateMachine& gsm) {
    // Day 1 events
}

void EventManager::RegisterDay5Events(AntoniAI& a, CustomerAI& c, MarketMap& map, AudioManager& audio, GameStateMachine& gsm) {
    // Day 5 events
}

void EventManager::RegisterSecretEvents(AntoniAI& a, CustomerAI& c1, CustomerAI& c2, CustomerAI& c3, MarketMap& map, AudioManager& audio, GameStateMachine& gsm, Camera& cam) {
    // 1. Shooting Antoni
    RegisterEvent({"SHOOT_ANTONI", 
        [&](){ return gsm.Data().antoniChasing && GlobalContext::Get().player->inventory.hasPistol && InputManager::Get().IsMouseButtonPressed(1); },
        [&](){ 
            audio.PlaySFX("assets/audio/sfx_gunshot.wav");
            a.Reset(); 
            gsm.Data().antoniChasing = false;
        }
    });

    // 2. Calling the Police
    RegisterEvent({"CALL_POLICE",
        [&](){ return !gsm.Data().antoniChasing && glm::distance(cam.Position(), map.GetBillboardPos()) < 5.0f; },
        [&](){
            gsm.TransitionTo(GameScene::WIN_SCREEN);
        }
    });
}
