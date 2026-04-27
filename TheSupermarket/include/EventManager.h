#pragma once
#include <functional>
#include <queue>
#include <string>
#include <vector>

struct StoryEvent {
    std::string id;
    std::function<bool()>  trigger;
    std::function<void()>  action;
    bool                   fired = false;
    bool                   blocking = false;
};

class EventManager {
public:
    void RegisterEvent(StoryEvent event);
    void Update(float dt);
    bool IsBlocking() const;
    void SkipCurrent();

    void RegisterDay1Events(class AntoniAI& a, class CustomerAI& c,
                            class AudioManager& audio, class GameStateMachine& gsm);
    void RegisterDay5Events(class AntoniAI& a, class CustomerAI& c,
                            class MarketMap& map, class AudioManager& audio,
                            class GameStateMachine& gsm);
    void RegisterSecretEvents(class AntoniAI& a, class CustomerAI& c1,
                              class CustomerAI& c2, class CustomerAI& c3,
                              class MarketMap& map, class AudioManager& audio,
                              class GameStateMachine& gsm, class Camera& cam);

private:
    std::vector<StoryEvent> events_;
    bool blocking_ = false;
    float blockTimer_ = 0.0f;
};
