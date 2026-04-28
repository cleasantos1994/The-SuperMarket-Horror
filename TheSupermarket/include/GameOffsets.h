#pragma once
#include <cstddef>
#include "GlobalContext.h"
#include "GameState.h"
#include "Player.h"
#include "AntoniAI.h"
#include "CustomerAI.h"

struct GameOffsets {
    // External (Bases / Global Instances)
    size_t GlobalContext_Base;
    size_t GameStateMachine_Base;
    size_t Player_Base;

    // Internal - GlobalContext Member Offsets
    size_t GlobalContext_GSM;
    size_t GlobalContext_Player;
    size_t GlobalContext_Antoni;
    size_t GlobalContext_Characters;

    // Internal - Player Member Offsets
    size_t Player_Camera;
    size_t Player_Inventory;
    size_t Player_FearLevel;
    size_t Player_Stamina;
    size_t Player_IsSprinting;
    size_t Player_IsSlipping;
    size_t Player_SlipTimer;

    // Internal - Items Member Offsets
    size_t Items_HasPistol;
    size_t Items_HasCarKeys;
    size_t Items_Bullets;
    size_t Items_OilJugCount;

    // Internal - Camera Member Offsets (from Camera base)
    size_t Camera_Position;
    size_t Camera_Front;
    size_t Camera_Yaw;
    size_t Camera_Pitch;

    // Internal - AntoniAI Member Offsets
    size_t Antoni_State;
    size_t Antoni_Position;
    size_t Antoni_ChaseSpeed;
    size_t Antoni_IsDisguised;
    size_t Antoni_HasOilJug;

    // Internal - GameStateData Member Offsets
    size_t GameState_CurrentScene;
    size_t GameState_PlayTime;
    size_t GameState_DeathCount;
    size_t GameState_AntoniChasing;

    // Internal - CustomerAI Member Offsets
    size_t Customer_Type;
    size_t Customer_State;
    size_t Customer_Position;
    size_t Customer_HasPooped;
};

inline GameOffsets GetGameOffsets() {
    GameOffsets o;
    // External Bases
    o.GlobalContext_Base = (size_t)&GlobalContext::Get();
    o.GameStateMachine_Base = (size_t)&GameStateMachine::Get();
    o.Player_Base = (size_t)GlobalContext::Get().player;

    // GlobalContext
    o.GlobalContext_GSM = offsetof(GlobalContext, gsm);
    o.GlobalContext_Player = offsetof(GlobalContext, player);
    o.GlobalContext_Antoni = offsetof(GlobalContext, antoni);
    o.GlobalContext_Characters = offsetof(GlobalContext, activeCustomers);

    // Player
    o.Player_Camera = offsetof(Player, camera);
    o.Player_Inventory = offsetof(Player, inventory);
    o.Player_FearLevel = offsetof(Player, fearLevel);
    o.Player_Stamina = offsetof(Player, stamina);
    o.Player_IsSprinting = offsetof(Player, isSprinting);
    o.Player_IsSlipping = offsetof(Player, isSlipping);
    o.Player_SlipTimer = offsetof(Player, slipTimer);

    // Items
    o.Items_HasPistol = offsetof(Items, hasPistol);
    o.Items_HasCarKeys = offsetof(Items, hasCarKeys);
    o.Items_Bullets = offsetof(Items, bullets);
    o.Items_OilJugCount = offsetof(Items, oilJugCount);

    // GameStateData
    o.GameState_CurrentScene = offsetof(GameStateData, currentScene);
    o.GameState_PlayTime = offsetof(GameStateData, playTime);
    o.GameState_DeathCount = offsetof(GameStateData, deathCount);
    o.GameState_AntoniChasing = offsetof(GameStateData, antoniChasing);

    // Hardcoded private offsets for Camera, AntoniAI, CustomerAI
    // These are stable due to the fixed class definitions.
    o.Camera_Position = 0;
    o.Camera_Front = 12;
    o.Camera_Yaw = 48;
    o.Camera_Pitch = 52;

    o.Antoni_State = 8;
    o.Antoni_Position = 12;
    o.Antoni_ChaseSpeed = 96; 
    o.Antoni_IsDisguised = 109; 
    o.Antoni_HasOilJug = 108;

    o.Customer_Type = 0;
    o.Customer_State = 4;
    o.Customer_Position = 16;
    o.Customer_HasPooped = 96;

    return o;
}
