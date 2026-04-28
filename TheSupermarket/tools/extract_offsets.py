import subprocess
import json
import sys
import os
import re

def extract_offsets(binary_path, output_json):
    if not os.path.exists(binary_path):
        print(f"Error: Binary {binary_path} not found.")
        return

    try:
        # Use nm -C to get demangled symbols
        result = subprocess.run(['nm', '-C', binary_path], capture_output=True, text=True)
        if result.returncode != 0:
            print("Error running nm")
            return
        
        symbols = {}
        for line in result.stdout.splitlines():
            parts = line.split()
            if len(parts) >= 3:
                addr = parts[0]
                name = " ".join(parts[2:])
                symbols[name] = f"0x{addr}"

        # Find the GameStateMachine singleton instance
        # Typically looks like: GameStateMachine::Get()::instance
        gsm_instance = None
        for name, addr in symbols.items():
            if "GameStateMachine::Get()::instance" in name or "GameStateMachine::state_" in name:
                gsm_instance = addr
                break
        
        # Define manual offsets for GameStateData members based on GameState.h
        # (Assuming 4-byte alignment/size for enums/floats/ints)
        game_state_offsets = {
            "currentScene": "0x0",
            "previousScene": "0x4",
            "loadProgress": "0x8",
            "day1Task": "0xC",
            "day5Task": "0x10",
            "day1TasksDone": "0x14",
            "day5TasksDone": "0x18",
            "hasCarKeys": "0x1C",
            "hasPistol": "0x1D",
            "antoniFocusedOnPlayer": "0x1E",
            "fearLevel": "0x20",
            "antoniChasing": "0x24",
            "isPaused": "0x25",
            "playTime": "0x28",
            "deathCount": "0x2C",
            "isSlipping": "0x30",
            "slipTimer": "0x34",
            "masterVolume": "0x38",
            "mouseSensitivity": "0x3C",
            "calebAlive": "0x44",
            "williamAlive": "0x45",
            "guilhermeAlive": "0x46"
        }

        datamodel = {
            "GameDatamodel": {
                "Base": gsm_instance if gsm_instance else "0x0",
                "Description": "Memory map for The Supermarket Game State",
                "Singletons": {
                    "GameStateMachine": gsm_instance
                },
                "Offsets": {
                    "GameStateData": game_state_offsets
                },
                "Symbols": {
                    "main": symbols.get("main", "Not Found"),
                    "AudioManager": symbols.get("AudioManager::Get()::instance", "Not Found"),
                    "InputManager": symbols.get("InputManager::Get()::instance", "Not Found")
                }
            }
        }
        
        with open(output_json, 'w') as f:
            json.dump(datamodel, f, indent=4)
        print(f"Improved offsets extracted to {output_json}")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_offsets.py <binary_path> <output_json>")
    else:
        extract_offsets(sys.argv[1], sys.argv[2])
