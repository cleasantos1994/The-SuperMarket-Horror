#include "InputManager.h"

void InputManager::Init(GLFWwindow* window) {
    window_ = window;
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::PollEvents() {
    glfwPollEvents();
    
    double xpos, ypos;
    glfwGetCursorPos(window_, &xpos, &ypos);
    
    if (firstMouse_) {
        mousePos_.x = xpos;
        mousePos_.y = ypos;
        firstMouse_ = false;
    }
    
    mouseDelta_.x = xpos - mousePos_.x;
    mouseDelta_.y = mousePos_.y - ypos; // reversed since y-coordinates go from bottom to top
    
    mousePos_.x = xpos;
    mousePos_.y = ypos;
}

void InputManager::EndFrame() {
    mouseDelta_ = glm::vec2(0.0f);
}

bool InputManager::IsKeyPressed(int key) const {
    return glfwGetKey(window_, key) == GLFW_PRESS;
}

bool InputManager::IsMouseButtonPressed(int button) const {
    return glfwGetMouseButton(window_, button) == GLFW_PRESS;
}

void InputManager::SetCursorLocked(bool locked) {
    if (locked) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse_ = true; // reset first mouse flag so camera doesn't jump
    } else {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void InputManager::Navigate(int& dx, int& dy) {
    static bool upPressed = false;
    static bool downPressed = false;
    
    if (IsKeyPressed(GLFW_KEY_UP) || IsKeyPressed(GLFW_KEY_W)) {
        if (!upPressed) { dy = -1; upPressed = true; }
        else dy = 0;
    } else {
        upPressed = false;
        dy = 0;
    }
    
    if (IsKeyPressed(GLFW_KEY_DOWN) || IsKeyPressed(GLFW_KEY_S)) {
        if (!downPressed) { dy = 1; downPressed = true; }
        else dy = 0;
    } else {
        downPressed = false;
    }
}

bool InputManager::Confirm() {
    static bool enterPressed = false;
    if (IsKeyPressed(GLFW_KEY_ENTER) || IsKeyPressed(GLFW_KEY_SPACE)) {
        if (!enterPressed) { enterPressed = true; return true; }
    } else {
        enterPressed = false;
    }
    return false;
}

bool InputManager::Interact() {
    static bool ePressed = false;
    if (IsKeyPressed(GLFW_KEY_E)) {
        if (!ePressed) { ePressed = true; return true; }
    } else {
        ePressed = false;
    }
    return false;
}

bool InputManager::Sprint() { return IsKeyPressed(GLFW_KEY_LEFT_SHIFT); }
bool InputManager::MoveForward() { return IsKeyPressed(GLFW_KEY_W); }
bool InputManager::MoveBackward() { return IsKeyPressed(GLFW_KEY_S); }
bool InputManager::MoveLeft() { return IsKeyPressed(GLFW_KEY_A); }
bool InputManager::MoveRight() { return IsKeyPressed(GLFW_KEY_D); }