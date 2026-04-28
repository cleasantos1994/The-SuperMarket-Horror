#include "AntiCheat.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <csignal>
#include "GlobalContext.h"
#include "Player.h"

#ifdef __linux__
#include <sys/ptrace.h>
#endif

void SimpleAntiCheat::Init() {
    std::cout << "[AntiCheat] Initializing Security Modules..." << std::endl;
    // Initial security scan
    if (!SecurityCheck()) {
        LogAndCrash("Initial security check failed.");
    }
}

void SimpleAntiCheat::Update(float dt) {
    checkTimer_ += dt;
    if (checkTimer_ > 5.0f) { // Check every 5 seconds
        checkTimer_ = 0.0f;
        if (!SecurityCheck()) {
            LogAndCrash("Periodic security check failed.");
        }
        if (IsDebuggerPresent()) {
            LogAndCrash("Debugger detected.");
        }

        // Honeypot checks
        auto player = GlobalContext::Get().player;
        if (player) {
            if (player->hp_padding1 != 0xDEADC0DE || 
                player->hp_padding2 != 0xCAFEBABE ||
                player->hp_speed_mod != 1.0f) {
                LogAndCrash("Memory integrity violation (Honeypot triggered).");
            }
        }
    }
}

bool SimpleAntiCheat::SecurityCheck() {
    // On Linux/Android, we scan /proc/self/maps for injected libraries
    std::ifstream maps("/proc/self/maps");
    if (!maps.is_open()) return true;

    std::string line;
    std::vector<std::string> blacklist = {
        "libcheat", "GameGuardian", "Frida", "Xposed", "Substrate"
    };

    while (std::getline(maps, line)) {
        for (const auto& item : blacklist) {
            if (line.find(item) != std::string::npos) {
                return false;
            }
        }
    }
    return true;
}

bool SimpleAntiCheat::IsDebuggerPresent() {
#ifdef __linux__
    // Try to PTRACE_TRACEME. If it fails, a debugger is already attached.
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        return true;
    }
    ptrace(PTRACE_DETACH, 0, 1, 0);
#endif
    return false;
}

void SimpleAntiCheat::LogAndCrash(const std::string& reason) {
    std::ofstream log("ac_log.txt", std::ios::app);
    if (log.is_open()) {
        log << "[SECURITY ALERT] Detection Reason: " << reason << std::endl;
        log.close();
    }
    std::cerr << "[CRITICAL] Security violation detected. Crashing for safety." << std::endl;
    // Crash the game
    std::raise(SIGABRT);
}
