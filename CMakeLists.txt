cmake_minimum_required(VERSION 3.10)
project(MarketHorror_Antoni)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
# Note: In a real environment, you'd need to ensure GLFW and GLM are available.
# This is a placeholder structure based on the GEMINI.md spec.

add_executable(MarketHorror_Antoni src/main.cpp)
target_link_libraries(MarketHorror_Antoni OpenGL::GL)
