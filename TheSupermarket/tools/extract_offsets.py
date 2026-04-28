import subprocess
import json
import sys
import os

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

        # Revamped Internal Offsets for GlobalContext
        global_ctx_instance = None
        for name, addr in symbols.items():
            if "GlobalContext::Get()::instance" in name:
                global_ctx_instance = addr
                break
        
        # Define layouts for internal systems
        global_ctx_offsets = {
            "gsm": "0x0",
            "audio": "0x8",
            "input": "0x10",
            "ui": "0x18",
            "camera": "0x20",
            "market": "0x28",
            "antoni": "0x30",
            "tasks": "0x38",
            "events": "0x40",
            "particles": "0x48",
            "post": "0x50"
        }

        datamodel = {
            "Internal_Offsets": {
                "GlobalContext_Base": global_ctx_instance if global_ctx_instance else "0x0",
                "GlobalContext_Layout": global_ctx_offsets,
                "GameStateMachine_State": symbols.get("GameStateMachine::state_", "Not Found"),
                "Singletons": {
                    "AudioManager": symbols.get("AudioManager::Get()::instance", "Not Found"),
                    "InputManager": symbols.get("InputManager::Get()::instance", "Not Found"),
                    "GlobalContext": global_ctx_instance
                },
                "Core_Functions": {
                    "main": symbols.get("main", "Not Found"),
                    "GameStateMachine_TransitionTo": [k for k in symbols if "GameStateMachine::TransitionTo" in k],
                    "AntoniAI_Update": [k for k in symbols if "AntoniAI::Update" in k]
                }
            }
        }
        
        with open(output_json, 'w') as f:
            json.dump(datamodel, f, indent=4)
        print(f"Internal offsets extracted to {output_json}")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_offsets.py <binary_path> <output_json>")
    else:
        extract_offsets(sys.argv[1], sys.argv[2])
