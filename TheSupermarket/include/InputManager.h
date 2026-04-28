#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>

#include "Export.h"

class INPUT_API InputManager {
public:
    static InputManager& Get() {
        static InputManager instance;
        return instance;
    }

    void Init();
    void PollEvents(bool& quit);
    void EndFrame();

    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsMouseButtonPressed(uint8_t button) const;
    glm::vec2 MousePosition() const { return mousePos_; }
    glm::vec2 MouseDelta() const { return mouseDelta_; }

    void SetCursorLocked(bool locked);
    void Navigate(int& dx, int& dy);
    bool Confirm();
    bool Interact();
    bool Sprint();
    bool MoveForward();
    bool MoveBackward();
    bool MoveLeft();
    bool MoveRight();

private:
    InputManager() = default;
    glm::vec2   mousePos_{0};
    glm::vec2   mouseDelta_{0};
    bool        firstMouse_ = true;
    const uint8_t* keyboardState_ = nullptr;
    uint32_t mouseState_ = 0;
    std::map<SDL_Scancode, bool> prevKeyboardState_;
};
