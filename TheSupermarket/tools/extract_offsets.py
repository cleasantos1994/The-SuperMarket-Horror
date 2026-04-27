import subprocess
import json
import sys
import os

def extract_offsets(binary_path, output_json):
    if not os.path.exists(binary_path):
        print(f"Error: Binary {binary_path} not found.")
        return

    # Use nm to get symbols and their addresses
    try:
        result = subprocess.run(['nm', '-C', '--numeric-sort', binary_path], capture_output=True, text=True)
        if result.returncode != 0:
            print("Error running nm")
            return
        
        offsets = {}
        for line in result.stdout.splitlines():
            parts = line.split()
            if len(parts) >= 3:
                addr = parts[0]
                type = parts[1]
                name = " ".join(parts[2:])
                # Only include relevant symbols (e.g., global data, functions)
                if type.lower() in ['t', 'd', 'b', 'v', 'w']:
                    offsets[name] = f"0x{addr}"
        
        with open(output_json, 'w') as f:
            json.dump(offsets, f, indent=4)
        print(f"Offsets extracted to {output_json}")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_offsets.py <binary_path> <output_json>")
    else:
        extract_offsets(sys.argv[1], sys.argv[2])
