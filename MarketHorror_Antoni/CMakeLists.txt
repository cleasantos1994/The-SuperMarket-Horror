cmake_minimum_required(VERSION 3.20)
project(MarketHorror_Antoni VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

include(FetchContent)

# GLFW
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.9)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
    set(GLFW_LIBRARIES glfw)
else()
    set(GLFW_LIBRARIES glfw)
endif()

# GLAD
FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG v0.1.36)
FetchContent_MakeAvailable(glad)

# GLM
find_package(glm QUIET)
if(NOT glm_FOUND)
    FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
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
add_executable(MarketHorror_Antoni ${SOURCES})

target_include_directories(MarketHorror_Antoni PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/extern/miniaudio
  ${stb_SOURCE_DIR}
  ${glad_SOURCE_DIR}/include)

target_link_libraries(MarketHorror_Antoni PRIVATE
  ${GLFW_LIBRARIES} glad glm assimp nlohmann_json::nlohmann_json ${CMAKE_DL_LIBS})

add_custom_command(TARGET MarketHorror_Antoni POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:MarketHorror_Antoni>/assets)
