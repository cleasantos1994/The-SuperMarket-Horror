#include "EventManager.h"
#include "AntoniAI.h"
#include "CustomerAI.h"
#include "AudioManager.h"
#include "GameState.h"
#include "MarketMap.h"

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
                blockTimer_ = 5.0f;
            }
        }
    }
    
    if (blocking_) {
        blockTimer_ -= dt;
        if (blockTimer_ <= 0) blocking_ = false;
    }
}

bool EventManager::IsBlocking() const { return blocking_; }
void EventManager::SkipCurrent() { blocking_ = false; }

void EventManager::RegisterDay1Events(AntoniAI& a, CustomerAI& c, AudioManager& audio, GameStateMachine& gsm) {
    RegisterEvent({"POOP_FLOOR", 
        [&gsm](){ return gsm.Data().day1TasksDone >= 3; },
        [&gsm, &c, &a](){
            c.TriggerIncident();
            gsm.TransitionTo(GameScene::CUTSCENE_POOP1);
        }, true});
    
    RegisterEvent({"ANTONI_KIDNAP",
        [&gsm](){ return gsm.Current() == GameScene::CUTSCENE_POOP1; },
        [&a, &c](){
            a.Materialize({5, 0, -10});
            a.BeginKidnap(c.Pos());
            c.BeginKidnap();
        }, false});
}

void EventManager::RegisterDay5Events(AntoniAI& a, CustomerAI& c, MarketMap& map, AudioManager& audio, GameStateMachine& gsm) {
    RegisterEvent({"POOP_REGISTER",
        [&gsm](){ return gsm.Data().day5TasksDone >= 3; },
        [&gsm, &c](){
            c.TriggerIncident();
            gsm.TransitionTo(GameScene::CUTSCENE_POOP2);
        }, true});
}