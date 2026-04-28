#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
    #define GAME_EXPORT __declspec(dllexport)
    #define GAME_IMPORT __declspec(dllimport)
#else
    #define GAME_EXPORT __attribute__ ((visibility ("default")))
    #define GAME_IMPORT
#endif

#ifdef GAME_CORE_BUILD
    #define GAMECORE_API GAME_EXPORT
#else
    #define GAMECORE_API GAME_IMPORT
#endif

#ifdef USER_INPUT_BUILD
    #define INPUT_API GAME_EXPORT
#else
    #define INPUT_API GAME_IMPORT
#endif

#ifdef UI_RENDERER_BUILD
    #define UI_API GAME_EXPORT
#else
    #define UI_API GAME_IMPORT
#endif

#ifdef ANTI_CHEAT_BUILD
    #define AC_API GAME_EXPORT
#else
    #define AC_API GAME_IMPORT
#endif
