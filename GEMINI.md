# MARKET HORROR: THE LEGEND OF ANTONI

## Project Overview
**MARKET HORROR: THE LEGEND OF ANTONI** is a First-Person Horror/Comedy/Meme Simulator developed using C++20 and OpenGL 3.3. The project is currently in a "Specification Phase," where the entire game design, technical architecture, and implementation roadmap are defined within a single comprehensive prompt file (`xjBdC34D`).

The game puts the player in the role of a shelf stocker at "SUPERMEME MARKET™" who must survive shifts involving strange customers, "poop incidents," and a mysterious antagonist named Antoni who smells of baby oil.

## Technical Stack
- **Language:** C++20
- **Graphics API:** OpenGL 3.3 (Core Profile)
- **Libraries:** GLFW (Windowing/Input), GLM (Mathematics), Assimp (Model Loading)
- **Build System:** CMake
- **Audio/TTS:** `espeak-ng`, `ffmpeg`, `gTTS` (Python)
- **Asset Processing:** ImageMagick (`convert`)

## Project Structure (Target)
The following structure is defined in the specification and should be followed during initialization:
```
MarketHorror_Antoni/
├── src/                # Game loop, State Machine, AI, Renderers
├── include/            # Header files
├── assets/             # 3D Models, Textures, Shaders, Audio
├── CMakeLists.txt      # Build configuration
└── init.txt            # Initialization log
```

## Building and Running
### Prerequisites
Ensure the following dependencies are installed (Ubuntu/Debian example):
```bash
sudo apt-get install build-essential cmake libgl1-mesa-dev libglu1-mesa-dev \
                     libx11-dev libglfw3-dev libassimp-dev espeak-ng ffmpeg
```

### Build Instructions (Inferred)
1. Initialize the project structure as defined in Phase 1 of `xjBdC34D`.
2. Generate/Download assets as defined in Phase 2.
3. Compile using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ./MarketHorror_Antoni
   ```

## Development Conventions
- **Architecture:** Follow the modular structure outlined in `xjBdC34D` (e.g., separate managers for Audio, Input, AI, and Tasks).
- **Aesthetic:** High-contrast horror/meme aesthetic (Red/Black/White), bitmap fonts, and specific "meme" triggers.
- **Physics:** Implement custom "baby oil" slip hazards as described in the chase mechanics.
- **Narrative Integrity:** Adhere to the "Narrative Bible" in the spec, including the scripted "poop events" and Antoni's AI behaviors.

## Key Files
- `xjBdC34D`: The "Full Game Agent Prompt v3.0" which serves as the primary source of truth for all code, assets, and narrative.
- `GEMINI.md`: This file.
