#pragma once
#include <cstddef>
#include "GlobalContext.h"
#include "GameState.h"
#include "Player.h"
#include "AntoniAI.h"

struct GameOffsets {
    // Bases (Global Instances)
    size_t GlobalContext_Base;
    size_t GameStateMachine_Base;
    size_t Player_Base;

    // GlobalContext Member Offsets
    size_t GlobalContext_GSM;
    size_t GlobalContext_Player;
    size_t GlobalContext_Antoni;
    size_t GlobalContext_Characters;

    // Player Member Offsets
    size_t Player_Camera;
    size_t Player_Inventory;
    size_t Player_FearLevel;
    size_t Player_IsSlipping;

    // Items Member Offsets
    size_t Items_HasPistol;
    size_t Items_HasCarKeys;

    // Camera Member Offsets (from Camera base)
    size_t Camera_Position;
    size_t Camera_Front;

    // AntoniAI Member Offsets
    size_t Antoni_Position;
    size_t Antoni_State;
};

inline GameOffsets GetGameOffsets() {
    GameOffsets o;
    // Pointers (Addresses relative to binary load base will be handled by dumper)
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
    o.Player_IsSlipping = offsetof(Player, isSlipping);

    // Items
    o.Items_HasPistol = offsetof(Items, hasPistol);
    o.Items_HasCarKeys = offsetof(Items, hasCarKeys);

    // Camera (Note: Camera fields are private, we might need friend or public access)
    // For now, assume we make them public for the dumper or use a hack.
    // Better: Make the dumper a friend.
    o.Camera_Position = 0; // pos_ is first
    o.Camera_Front = 12;   // front_ is after pos_ (3 * 4)

    // AntoniAI
    o.Antoni_Position = 12; // In AntoniAI.h, pos_ is after state_ (int) and model_ (ptr)
    // Wait, let's check AntoniAI.h to be sure.
    o.Antoni_State = 8;

    return o;
}
