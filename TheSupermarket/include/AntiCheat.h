#pragma once
#include <string>
#include <vector>

class SimpleAntiCheat {
public:
    static SimpleAntiCheat& Get() {
        static SimpleAntiCheat instance;
        return instance;
    }

    void Init();
    void Update(float dt);
    
    // Detects code injection or unauthorized modules
    bool SecurityCheck();
    
    // Detects debugger attachment
    bool IsDebuggerPresent();

private:
    SimpleAntiCheat() = default;
    float checkTimer_ = 0.0f;
    void LogAndCrash(const std::string& reason);
    std::vector<std::string> knownModules_;
};
