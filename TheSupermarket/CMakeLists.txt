cmake_minimum_required(VERSION 3.20)
project(TheSupermarket VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

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

# miniaudio
FetchContent_Declare(miniaudio
  URL https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
  DOWNLOAD_NO_EXTRACT TRUE
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/extern/miniaudio)
FetchContent_MakeAvailable(miniaudio)

# stb
FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG master)
FetchContent_MakeAvailable(stb)

# nlohmann/json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(nlohmann_json
      GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.11.3)
    FetchContent_MakeAvailable(nlohmann_json)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")

if(ANDROID)
    add_library(main SHARED ${SOURCES})
    set(GAME_TARGET main)
else()
    add_executable(TheSupermarket ${SOURCES})
    set(GAME_TARGET TheSupermarket)
endif()

target_include_directories(${GAME_TARGET} PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/extern/miniaudio
  ${stb_SOURCE_DIR}
  ${glad_SOURCE_DIR}/include)

target_link_libraries(${GAME_TARGET} PRIVATE
  ${SDL2_LIBRARIES} glad ${ASSIMP_LIBRARIES} nlohmann_json::nlohmann_json ${CMAKE_DL_LIBS})

# Handle GLM target vs system include
if(TARGET glm::glm)
    target_link_libraries(${GAME_TARGET} PRIVATE glm::glm)
elseif(TARGET glm)
    target_link_libraries(${GAME_TARGET} PRIVATE glm)
endif()

if(NOT ANDROID)
    add_custom_command(TARGET TheSupermarket POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:TheSupermarket>/assets)
endif()
