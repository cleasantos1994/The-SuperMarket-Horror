import os
import subprocess

def run(cmd):
    print(f"Running: {cmd}")
    subprocess.run(cmd, shell=True, check=True)

def generate_textures():
    print("Generating High-Quality Textures...")
    # Better floor tiles
    run("magick -size 512x512 tile:checkerboard -fill '#a8a8a8' -draw 'color 0,0 reset' -attenuate 0.1 +noise Gaussian TheSupermarket/assets/textures/world_skin.png")
    # Wood shelf texture with grain
    run("magick -size 256x256 xc:'#623b1c' -sparse-color Barycentric '0,0 #623b1c 256,256 #3a220d' -attenuate 0.2 +noise Multiplicative TheSupermarket/assets/textures/shelf_tex.png")
    # Poop texture with swirl and grit
    run("magick -size 128x128 radial-gradient:'#4b2c15-#26150a' -swirl 120 -attenuate 0.3 +noise Poisson TheSupermarket/assets/textures/poop_tex.png")
    # Oil puddle with iridescent sheen (simulated)
    run("magick -size 256x256 radial-gradient:'#ffffff-#777777' -alpha set -channel alpha -evaluate multiply 0.4 -attenuate 0.1 +noise Impulse TheSupermarket/assets/textures/oil_puddle.png")

def generate_audio():
    print("Generating Processed Audio...")
    lines = {
        "narr_day1": "Welcome to your first shift. It is a normal Tuesday. Keep the aisles clean.",
        "narr_stranger_enters": "A customer has entered. They look... unusual.",
        "sfx_antoni_catch": "I have found you.",
        "sfx_caleb_greet": "Hey, it is Caleb. Watch out for the mess in aisle four.",
        "bgm_menu": "Supermeme Market. Cleanup on aisle seven. Supermeme Market."
    }
    
    for filename, text in lines.items():
        wav_path = f"TheSupermarket/assets/audio/{filename}.wav"
        # Generate with espeak and then add some reverb/effect with ffmpeg
        run(f"espeak -v en -s 120 -p 40 '{text}' -w temp.wav")
        run(f"ffmpeg -i temp.wav -af 'aecho=0.8:0.8:40:0.5,lowpass=f=3000' {wav_path} -y")
    
    if os.path.exists("temp.wav"):
        os.remove("temp.wav")

def generate_models():
    print("Generating Improved Models (Basic Shapes)...")
    # Still basic OBJs, but can add slightly more vertices for 'grit'
    with open("TheSupermarket/assets/models/characters/entity_antoni.obj", "w") as f:
        f.write("v -0.5 0.0 0.0\nv 0.5 0.0 0.0\nv 0.5 2.5 0.0\nv -0.5 2.5 0.0\nv 0.0 2.8 0.5\nf 1 2 3 4\nf 1 4 5\n")

if __name__ == "__main__":
    os.makedirs("TheSupermarket/assets/textures", exist_ok=True)
    os.makedirs("TheSupermarket/assets/audio", exist_ok=True)
    os.makedirs("TheSupermarket/assets/models/characters", exist_ok=True)
    generate_textures()
    generate_audio()
    generate_models()
