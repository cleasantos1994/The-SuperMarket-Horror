import subprocess
import json
import sys
import os
import re

def get_symbols(binary_path):
    if not os.path.exists(binary_path):
        return {}
    try:
        # Use nm -C for demangled symbols, -D for dynamic symbols if it's a shared lib
        cmd = ['nm', '-C', binary_path]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            # Try dynamic if static fails
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

    global_ctx_instance = None
    for name, addr in all_symbols.items():
        if "GlobalContext::Get()::instance" in name:
            global_ctx_instance = addr
            break
    
    # "External" / Game State Offsets (based on GameStateData)
    # These are offsets from the 'state_' member in GameStateMachine
    game_state_offsets = {
        "currentScene": "0x0",
        "previousScene": "0x4",
        "loadProgress": "0x8",
        "day1TasksDone": "0x14",
        "fearLevel": "0x20",
        "antoniChasing": "0x24",
        "playTime": "0x28",
        "hasPistol": "0x1D"
    }

    # "Internal" / System Pointers (offsets from GlobalContext base)
    global_ctx_layout = {
        "gsm": "0x0",
        "audio": "0x8",
        "input": "0x10",
        "ui": "0x18",
        "camera": "0x20",
        "market": "0x28",
        "antoni": "0x30"
    }

    datamodel = {
        "GameDatamodel": {
            "Version": "4.0",
            "Internal": {
                "GlobalContext_Base": global_ctx_instance if global_ctx_instance else "0x0",
                "GlobalContext_Layout": global_ctx_layout
            },
            "External": {
                "GameState_Base": all_symbols.get("GameStateMachine::state_", "Not Found"),
                "GameState_Offsets": game_state_offsets
            },
            "Singletons": {
                "GameStateMachine": all_symbols.get("GameStateMachine::Get()::instance", "Not Found"),
                "AudioManager": all_symbols.get("AudioManager::Get()::instance", "Not Found"),
                "InputManager": all_symbols.get("InputManager::Get()::instance", "Not Found")
            },
            "Functions": {
                "main": all_symbols.get("main", "Not Found"),
                "AntiCheat_SecurityCheck": [k for k in all_symbols if "SimpleAntiCheat::SecurityCheck" in k],
                "AntoniAI_Update": [k for k in all_symbols if "AntoniAI::Update" in k]
            }
        }
    }
    
    with open(output_json, 'w') as f:
        json.dump(datamodel, f, indent=4)
    print(f"Comprehensive offsets extracted to {output_json}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_offsets.py <build_dir> <output_json>")
    else:
        extract_offsets(sys.argv[1], sys.argv[2])
