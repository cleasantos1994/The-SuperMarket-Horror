import subprocess
import json
import sys
import os

def extract_offsets(build_dir, output_json):
    dumper_path = os.path.join(build_dir, "OffsetDumper")
    if not os.path.exists(dumper_path):
        print(f"Error: {dumper_path} not found. Please build the project first.")
        return

    try:
        # Run the C++ offset dumper
        result = subprocess.run([dumper_path], capture_output=True, text=True, check=True)
        cpp_offsets = json.loads(result.stdout)
        
        # Merge with any other metadata if needed, here we just format it
        datamodel = {
            "GameDatamodel": {
                "Bases": {
                    "GlobalContext": cpp_offsets["Bases"]["GlobalContext_Ptr"],
                    "GameStateMachine": cpp_offsets["Bases"]["GameStateMachine_Ptr"],
                    "Player_Base": cpp_offsets["Bases"]["Player_Ptr"]
                },
                "Player": {
                    "Offset": cpp_offsets["Offsets"]["GlobalContext"]["player"],
                    "Camera_Offset": cpp_offsets["Offsets"]["Player"]["camera"],
                    "FearLevel_Offset": cpp_offsets["Offsets"]["Player"]["fearLevel"],
                    "IsSlipping_Offset": cpp_offsets["Offsets"]["Player"]["isSlipping"]
                },
                "Items": {
                    "Inventory_Offset": cpp_offsets["Offsets"]["Player"]["inventory"],
                    "HasPistol_Offset": cpp_offsets["Offsets"]["Items"]["hasPistol"],
                    "HasCarKeys_Offset": cpp_offsets["Offsets"]["Items"]["hasCarKeys"]
                },
                "Antoni": {
                    "Antoni_Ptr_Offset": cpp_offsets["Offsets"]["GlobalContext"]["antoni"],
                    "State_Offset": cpp_offsets["Offsets"]["AntoniAI"]["state"],
                    "Pos_Offset": cpp_offsets["Offsets"]["AntoniAI"]["position"]
                },
                "Characters": {
                    "Characters_Vector_Offset": cpp_offsets["Offsets"]["GlobalContext"]["characters"]
                },
                "Raw_CPP_Offsets": cpp_offsets["Offsets"]
            }
        }

        with open(output_json, 'w') as f:
            json.dump(datamodel, f, indent=4)
        print(f"Verified C++ offsets extracted to {output_json}")
    except Exception as e:
        print(f"Failed to run OffsetDumper: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_offsets.py <build_dir> <output_json>")
    else:
        extract_offsets(sys.argv[1], sys.argv[2])
