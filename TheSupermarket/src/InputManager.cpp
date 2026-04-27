#include "InputManager.h"
#include <iostream>

void InputManager::Init() {
    keyboardState_ = SDL_GetKeyboardState(NULL);
}

void InputManager::PollEvents(bool& quit) {
    mouseDelta_ = {0, 0};
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_MOUSEMOTION) {
            if (firstMouse_) {
                firstMouse_ = false;
            } else {
                mouseDelta_.x = (float)event.motion.xrel;
                mouseDelta_.y = (float)event.motion.yrel;
            }
            mousePos_.x = (float)event.motion.x;
            mousePos_.y = (float)event.motion.y;
        }
    }
    mouseState_ = SDL_GetMouseState(NULL, NULL);
}

void InputManager::EndFrame() {
    // No specific end frame logic needed for basic SDL2 keyboard state
}

bool InputManager::IsKeyPressed(SDL_Scancode key) const {
    return keyboardState_[key];
}

bool InputManager::IsMouseButtonPressed(uint8_t button) const {
    return (mouseState_ & SDL_BUTTON(button));
}

void InputManager::SetCursorLocked(bool locked) {
    SDL_SetRelativeMouseMode(locked ? SDL_TRUE : SDL_FALSE);
}

void InputManager::Navigate(int& dx, int& dy) {
    dx = 0; dy = 0;
    static bool upPressed = false;
    static bool downPressed = false;
    
    if (keyboardState_[SDL_SCANCODE_UP] || keyboardState_[SDL_SCANCODE_W]) {
        if (!upPressed) { dy = -1; upPressed = true; }
    } else { upPressed = false; }
    
    if (keyboardState_[SDL_SCANCODE_DOWN] || keyboardState_[SDL_SCANCODE_S]) {
        if (!downPressed) { dy = 1; downPressed = true; }
    } else { downPressed = false; }
}

bool InputManager::Confirm() {
    static bool enterPressed = false;
    if (keyboardState_[SDL_SCANCODE_RETURN] || keyboardState_[SDL_SCANCODE_SPACE]) {
        if (!enterPressed) { enterPressed = true; return true; }
    } else { enterPressed = false; }
    return false;
}

bool InputManager::Interact() {
    return keyboardState_[SDL_SCANCODE_E];
}

bool InputManager::Sprint() {
    return keyboardState_[SDL_SCANCODE_LSHIFT];
}

bool InputManager::MoveForward()  { return keyboardState_[SDL_SCANCODE_W] || keyboardState_[SDL_SCANCODE_UP]; }
bool InputManager::MoveBackward() { return keyboardState_[SDL_SCANCODE_S] || keyboardState_[SDL_SCANCODE_DOWN]; }
bool InputManager::MoveLeft()     { return keyboardState_[SDL_SCANCODE_A] || keyboardState_[SDL_SCANCODE_LEFT]; }
bool InputManager::MoveRight()    { return keyboardState_[SDL_SCANCODE_D] || keyboardState_[SDL_SCANCODE_RIGHT]; }
