cmake_minimum_required(VERSION 3.20)
project(TheSupermarket VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

# Dependencies
# SDL2
find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
    FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.28.x)
    FetchContent_MakeAvailable(SDL2)
    set(SDL2_LIBRARIES SDL2::SDL2)
else()
    set(SDL2_LIBRARIES SDL2::SDL2)
endif()

# GLAD
FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG v0.1.36)
FetchContent_MakeAvailable(glad)

# GLM
find_package(glm QUIET)
if(NOT glm_FOUND)
    FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1)
    FetchContent_MakeAvailable(glm)
endif()

# Assimp
find_package(assimp QUIET)
if(NOT assimp_FOUND)
    FetchContent_Declare(assimp GIT_REPOSITORY https://github.com/assimp/assimp.git GIT_TAG v5.3.1)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(assimp)
endif()
if(TARGET assimp::assimp)
    set(ASSIMP_LIBRARIES assimp::assimp)
else()
    set(ASSIMP_LIBRARIES assimp)
endif()

# stb
FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG master)
FetchContent_MakeAvailable(stb)
set(stb_INCLUDE_DIR ${stb_SOURCE_DIR})

# nlohmann/json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.11.3)
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# Core Module (Game Logic)
set(CORE_SOURCES 
    src/AntoniAI.cpp 
    src/AudioManager.cpp 
    src/Camera.cpp 
    src/CustomerAI.cpp 
    src/EventManager.cpp 
    src/MarketMap.cpp 
    src/ModelLoader.cpp 
    src/ParticleSystem.cpp 
    src/PostProcess.cpp 
    src/SaveLoad.cpp 
    src/Shader.cpp 
    src/TaskSystem.cpp
)

# Input Module
set(INPUT_SOURCES src/InputManager.cpp)

# UI Module
set(UI_SOURCES src/UIRenderer.cpp)

# Anti-Cheat Module
set(AC_SOURCES src/SimpleAntiCheat.cpp)

if(ANDROID)
    # On Android, combine everything into one libmain.so for simplicity
    add_library(main SHARED ${CORE_SOURCES} ${INPUT_SOURCES} ${UI_SOURCES} ${AC_SOURCES} src/main.cpp)
    set(GAME_TARGET main)
else()
    # PC Version: Modular Shared Libraries
    add_library(UserInput SHARED ${INPUT_SOURCES})
    add_library(UIRenderer SHARED ${UI_SOURCES})
    add_library(SimpleAntiCheat SHARED ${AC_SOURCES})
    add_library(GameCore SHARED ${CORE_SOURCES})
    
    add_executable(TheSupermarket src/main.cpp)
    target_link_libraries(TheSupermarket PRIVATE GameCore UserInput UIRenderer SimpleAntiCheat)
    set(GAME_TARGET TheSupermarket)
    
    # Define module list for later linking
    set(MODULES GameCore UserInput UIRenderer SimpleAntiCheat)
endif()

# Offset Dumper Target (Built as executable for all platforms for internal tooling)
add_executable(OffsetDumper src/OffsetDumper.cpp)
if(ANDROID)
    target_link_libraries(OffsetDumper PRIVATE main)
else()
    target_link_libraries(OffsetDumper PRIVATE GameCore UserInput UIRenderer SimpleAntiCheat)
    list(APPEND MODULES OffsetDumper)
endif()

# Common configuration for all targets
macro(configure_game_target target_name)
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern/miniaudio
        ${stb_INCLUDE_DIR}
        ${glad_SOURCE_DIR}/include
    )
    # Ensure stb_INCLUDE_DIR is available even if fetch happens late
    if(stb_INCLUDE_DIR)
        target_include_directories(${target_name} PUBLIC ${stb_INCLUDE_DIR})
    endif()
    target_link_libraries(${target_name} PRIVATE
        ${SDL2_LIBRARIES} glad ${ASSIMP_LIBRARIES} nlohmann_json::nlohmann_json ${CMAKE_DL_LIBS}
    )
    if(TARGET glm::glm)
        target_link_libraries(${target_name} PRIVATE glm::glm)
    elseif(TARGET glm)
        target_link_libraries(${target_name} PRIVATE glm)
    endif()
endmacro()

if(ANDROID)
    configure_game_target(main)
    configure_game_target(OffsetDumper)
else()
    foreach(module ${MODULES})
        configure_game_target(${module})
    endforeach()
    configure_game_target(TheSupermarket)
endif()

if(NOT ANDROID)
    add_custom_command(TARGET TheSupermarket POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:TheSupermarket>/assets)
endif()
