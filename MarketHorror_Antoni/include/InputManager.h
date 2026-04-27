#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager {
public:
    static InputManager& Get() {
        static InputManager instance;
        return instance;
    }

    void Init(GLFWwindow* window);
    void PollEvents();
    void EndFrame();

    bool IsKeyPressed(int key) const;
    bool IsMouseButtonPressed(int button) const;
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
    GLFWwindow* window_ = nullptr;
    glm::vec2   mousePos_{0};
    glm::vec2   mouseDelta_{0};
    bool        firstMouse_ = true;
};
