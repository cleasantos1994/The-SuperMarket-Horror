import subprocess
import json
import sys
import os

def extract_offsets(build_dir, output_json):
    build_dir = os.path.abspath(build_dir)
    output_json = os.path.abspath(output_json)
    dumper_name = "OffsetDumper.exe" if os.name == 'nt' else "OffsetDumper"
    dumper_path = os.path.join(build_dir, dumper_name)
    
    # Debug: Print paths
    print(f"Build Dir: {build_dir}")
    print(f"Dumper Path: {dumper_path}")
    print(f"Output Path: {output_json}")
        
    if not os.path.exists(dumper_path):
        print(f"Error: {dumper_path} not found. Please build the project first.")
        return

    try:
        # Run the C++ offset dumper
        result = subprocess.run([dumper_path], capture_output=True, text=True, check=True)
        cpp_offsets = json.loads(result.stdout)
        
        # The Dumper now handles the Internal/External structure.
        # We just wrap it in GameDatamodel for consistency if needed, 
        # but the user asked for { Internal: {}, External: {} } format.
        
        final_data = {
            "Internal": cpp_offsets["Internal"],
            "External": cpp_offsets["External"]
        }

        # Ensure output directory exists
        output_dir = os.path.dirname(output_json)
        if output_dir and not os.path.exists(output_dir):
            os.makedirs(output_dir)

        with open(output_json, 'w') as f:
            json.dump(final_data, f, indent=4)
        print(f"Restructured offsets extracted to {output_json}")
    except Exception as e:
        print(f"Failed to run OffsetDumper: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_offsets.py <build_dir> <output_json>")
    else:
        extract_offsets(sys.argv[1], sys.argv[2])
