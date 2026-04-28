#include <iostream>
#include <iomanip>
#include "GameOffsets.h"

int main() {
    // Note: To get absolute addresses, we'd need to subtract the image base.
    // However, for offsets from class bases, offsetof is enough.
    // For Global Instances, we just print their relative position or just the offsets.
    
    GameOffsets o = GetGameOffsets();

    std::cout << "{" << std::endl;
    std::cout << "  \"Bases\": {" << std::endl;
    std::cout << "    \"GlobalContext_Ptr\": \"0x" << std::hex << o.GlobalContext_Base << "\"," << std::endl;
    std::cout << "    \"GameStateMachine_Ptr\": \"0x" << std::hex << o.GameStateMachine_Base << "\"," << std::endl;
    std::cout << "    \"Player_Ptr\": \"0x" << std::hex << o.Player_Base << "\"" << std::endl;
    std::cout << "  }," << std::endl;

    std::cout << "  \"Offsets\": {" << std::endl;
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
    std::cout << "      \"isSlipping\": \"0x" << std::hex << o.Player_IsSlipping << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"Items\": {" << std::endl;
    std::cout << "      \"hasPistol\": \"0x" << std::hex << o.Items_HasPistol << "\"," << std::endl;
    std::cout << "      \"hasCarKeys\": \"0x" << std::hex << o.Items_HasCarKeys << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"Camera\": {" << std::endl;
    std::cout << "      \"position\": \"0x" << std::hex << o.Camera_Position << "\"," << std::endl;
    std::cout << "      \"front\": \"0x" << std::hex << o.Camera_Front << "\"" << std::endl;
    std::cout << "    }," << std::endl;

    std::cout << "    \"AntoniAI\": {" << std::endl;
    std::cout << "      \"state\": \"0x" << std::hex << o.Antoni_State << "\"," << std::endl;
    std::cout << "      \"position\": \"0x" << std::hex << o.Antoni_Position << "\"" << std::endl;
    std::cout << "    }" << std::endl;

    std::cout << "  }" << std::endl;
    std::cout << "}" << std::endl;

    return 0;
}
