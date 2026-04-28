import subprocess
import json
import sys
import os

def get_symbols(binary_path):
    if not os.path.exists(binary_path):
        return {}
    try:
        cmd = ['nm', '-C', binary_path]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            cmd = ['nm', '-C', '-D', binary_path]
            result = subprocess.run(cmd, capture_output=True, text=True)
        
        symbols = {}
        for line in result.stdout.splitlines():
            parts = line.split()
            if len(parts) >= 3:
                addr = parts[0]
                name = " ".join(parts[2:])
                symbols[name] = f"0x{addr}"
        return symbols
    except:
        return {}

def extract_offsets(build_dir, output_json):
    binary_path = os.path.join(build_dir, "TheSupermarket")
    lib_paths = [
        os.path.join(build_dir, "libGameCore.so"),
        os.path.join(build_dir, "libUserInput.so"),
        os.path.join(build_dir, "libUIRenderer.so"),
        os.path.join(build_dir, "libSimpleAntiCheat.so")
    ]

    all_symbols = get_symbols(binary_path)
    for lib in lib_paths:
        all_symbols.update(get_symbols(lib))

    # Find key bases
    global_ctx = all_symbols.get("GlobalContext::Get()::instance", "0x0")
    gsm_instance = all_symbols.get("GameStateMachine::Get()::instance", "0x0")
    
    # In our architecture, GameStateMachine::state_ is the first member, 
    # so GameState_Base == gsm_instance
    gamestate_base = gsm_instance

    # Structure Layouts (Offsets from class base)
    layouts = {
        "GlobalContext": {
            "gsm": "0x0",
            "audio": "0x8",
            "input": "0x10",
            "ui": "0x18",
            "camera": "0x20",
            "market": "0x28",
            "antoni": "0x30",
            "tasks": "0x38",
            "events": "0x40"
        },
        "GameStateData": {
            "currentScene": "0x0",
            "fearLevel": "0x20",
            "antoniChasing": "0x24",
            "hasPistol": "0x1D",
            "playTime": "0x28"
        },
        "Camera": {
            "position": "0x0", # pos_ is first
            "front": "0xC",
            "right": "0x24"
        },
        "AntoniAI": {
            "state": "0x8",
            "position": "0xC",
            "chaseSpeed": "0x20"
        }
    }

    datamodel = {
        "GameDatamodel": {
            "Bases": {
                "GlobalContext": global_ctx,
                "GameStateMachine": gsm_instance,
                "GameState_Base": gamestate_base
            },
            "Player": {
                "Camera_Ptr_Offset": layouts["GlobalContext"]["camera"],
                "Pos_Offset_In_Camera": layouts["Camera"]["position"]
            },
            "Antoni": {
                "Antoni_Ptr_Offset": layouts["GlobalContext"]["antoni"],
                "State_Offset": layouts["AntoniAI"]["state"],
                "Pos_Offset": layouts["AntoniAI"]["position"]
            },
            "Items": {
                "HasPistol_Offset": layouts["GameStateData"]["hasPistol"],
                "FearLevel_Offset": layouts["GameStateData"]["fearLevel"]
            },
            "Internal_Layouts": layouts
        }
    }
    
    with open(output_json, 'w') as f:
        json.dump(datamodel, f, indent=4)
    print(f"Detailed offsets extracted to {output_json}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_offsets.py <build_dir> <output_json>")
    else:
        extract_offsets(sys.argv[1], sys.argv[2])
