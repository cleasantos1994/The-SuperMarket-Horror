#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef GAME_BUILD_DLL
        #define GAME_API __declspec(dllexport)
    #else
        #define GAME_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        #define GAME_API __attribute__ ((visibility ("default")))
    #else
        #define GAME_API
    #endif
#endif
