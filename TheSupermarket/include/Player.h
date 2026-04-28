#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include <vector>

struct Items {
    bool hasPistol       = false;
    bool hasCarKeys      = false;
    int  bullets         = 0;
    int  oilJugCount     = 0;
};

class Player {
public:
    Player() : camera({0.f, 1.7f, 0.f}) {}

    Camera  camera;
    Items   inventory;
    int     fearLevel    = 0;
    float   stamina      = 100.0f;
    bool    isSprinting  = false;
    bool    isSlipping   = false;
    float   slipTimer    = 0.0f;

    // Anti-Cheat Honeypots
    uint32_t hp_padding1 = 0xDEADC0DE;
    float    hp_speed_mod = 1.0f; 
    uint32_t hp_padding2 = 0xCAFEBABE;

    glm::vec3 GetPosition() const { return camera.Position(); }
};
