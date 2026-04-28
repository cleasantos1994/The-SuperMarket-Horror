#define SDL_MAIN_HANDLED
#include <iostream>
#include <iomanip>
#include "GameOffsets.h"

int main() {
    GameOffsets o = GetGameOffsets();

    std::cout << "{" << std::endl;
    
    // External (Bases)
    std::cout << "  \"External\": {" << std::endl;
    std::cout << "    \"Bases\": {" << std::endl;
    std::cout << "      \"GlobalContext\": \"0x" << std::hex << o.GlobalContext_Base << "\"," << std::endl;
    std::cout << "      \"GameStateMachine\": \"0x" << std::hex << o.GameStateMachine_Base << "\"," << std::endl;
    std::cout << "      \"Player_Base\": \"0x" << std::hex << o.Player_Base << "\"" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "  }," << std::endl;

    // Internal (Offsets)
    std::cout << "  \"Internal\": {" << std::endl;
    
    std::cout << "    \"GlobalContext\": {" << std::endl;
    std::cout << "      \"gsm\": \"0x" << std::hex << o.GlobalContext_GSM << "\"," << std::endl;
    std::cout << "      \"player\": \"0x" << std::hex << o.GlobalContext_Player << "\"," << std::endl;
    std::cout << "      \"antoni\": \"0x" << std::hex << o.GlobalContext_Antoni << "\"," << std::endl;
    std::cout << "      \"characters\": \"0x" << std::hex << o.GlobalContext_Characters << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"Player\": {" << std::endl;
    std::cout << "      \"camera\": \"0x" << std::hex << o.Player_Camera << "\"," << std::endl;
    std::cout << "      \"inventory\": \"0x" << std::hex << o.Player_Inventory << "\"," << std::endl;
    std::cout << "      \"fearLevel\": \"0x" << std::hex << o.Player_FearLevel << "\"," << std::endl;
    std::cout << "      \"stamina\": \"0x" << std::hex << o.Player_Stamina << "\"," << std::endl;
    std::cout << "      \"isSprinting\": \"0x" << std::hex << o.Player_IsSprinting << "\"," << std::endl;
    std::cout << "      \"isSlipping\": \"0x" << std::hex << o.Player_IsSlipping << "\"," << std::endl;
    std::cout << "      \"slipTimer\": \"0x" << std::hex << o.Player_SlipTimer << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"Items\": {" << std::endl;
    std::cout << "      \"hasPistol\": \"0x" << std::hex << o.Items_HasPistol << "\"," << std::endl;
    std::cout << "      \"hasCarKeys\": \"0x" << std::hex << o.Items_HasCarKeys << "\"," << std::endl;
    std::cout << "      \"bullets\": \"0x" << std::hex << o.Items_Bullets << "\"," << std::endl;
    std::cout << "      \"oilJugCount\": \"0x" << std::hex << o.Items_OilJugCount << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"GameStateData\": {" << std::endl;
    std::cout << "      \"currentScene\": \"0x" << std::hex << o.GameState_CurrentScene << "\"," << std::endl;
    std::cout << "      \"playTime\": \"0x" << std::hex << o.GameState_PlayTime << "\"," << std::endl;
    std::cout << "      \"deathCount\": \"0x" << std::hex << o.GameState_DeathCount << "\"," << std::endl;
    std::cout << "      \"antoniChasing\": \"0x" << std::hex << o.GameState_AntoniChasing << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"Camera\": {" << std::endl;
    std::cout << "      \"position\": \"0x" << std::hex << o.Camera_Position << "\"," << std::endl;
    std::cout << "      \"front\": \"0x" << std::hex << o.Camera_Front << "\"," << std::endl;
    std::cout << "      \"yaw\": \"0x" << std::hex << o.Camera_Yaw << "\"," << std::endl;
    std::cout << "      \"pitch\": \"0x" << std::hex << o.Camera_Pitch << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"AntoniAI\": {" << std::endl;
    std::cout << "      \"state\": \"0x" << std::hex << o.Antoni_State << "\"," << std::endl;
    std::cout << "      \"position\": \"0x" << std::hex << o.Antoni_Position << "\"," << std::endl;
    std::cout << "      \"chaseSpeed\": \"0x" << std::hex << o.Antoni_ChaseSpeed << "\"," << std::endl;
    std::cout << "      \"isDisguised\": \"0x" << std::hex << o.Antoni_IsDisguised << "\"," << std::endl;
    std::cout << "      \"hasOilJug\": \"0x" << std::hex << o.Antoni_HasOilJug << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"CustomerAI\": {" << std::endl;
    std::cout << "      \"type\": \"0x" << std::hex << o.Customer_Type << "\"," << std::endl;
    std::cout << "      \"state\": \"0x" << std::hex << o.Customer_State << "\"," << std::endl;
    std::cout << "      \"position\": \"0x" << std::hex << o.Customer_Position << "\"," << std::endl;
    std::cout << "      \"hasPooped\": \"0x" << std::hex << o.Customer_HasPooped << "\"" << std::endl;
    std::cout << "    }" << std::endl;

    std::cout << "  }" << std::endl;
    std::cout << "}" << std::endl;

    return 0;
}
