#include "Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 startPos, float yaw, float pitch)
    : pos_(startPos), yaw_(yaw), pitch_(pitch) {
    UpdateVectors();
}

void Camera::Update(float dx, float dy, float sensitivity) {
    if (slipping_) return;

    yaw_ += dx * sensitivity;
    pitch_ += dy * sensitivity;

    pitch_ = std::clamp(pitch_, -89.0f, 89.0f);
    UpdateVectors();
}

void Camera::Move(glm::vec3 direction, float speed, float dt) {
    pos_ += direction * speed * dt;
}

void Camera::Bob(bool isMoving, float dt) {
    if (isMoving && !slipping_) {
        bobPhase_ += dt * 10.0f;
        pos_.y = 1.7f + sin(bobPhase_) * 0.05f;
    } else {
        pos_.y = 1.7f;
    }
}

void Camera::ApplyFearEffect(int fear, float time) {
    if (fear > 50) {
        tiltOffset_ = sin(time * 5.0f) * ((fear - 50) / 50.0f) * 2.0f; // slight roll tilt
    } else {
        tiltOffset_ = 0.0f;
    }
}

void Camera::ApplySlip(float dt) {
    if (!slipping_) {
        slipping_ = true;
        slipVelX_ = front_.x * 2.0f;
        slipVelZ_ = front_.z * 2.0f;
    }
    pos_.x += slipVelX_ * dt;
    pos_.z += slipVelZ_ * dt;
    
    // Uncontrollable rotation
    yaw_ += dt * 90.0f;
    UpdateVectors();
}

void Camera::StopSlip() {
    slipping_ = false;
}

glm::mat4 Camera::GetViewMatrix() const {
    // Apply tilt
    glm::mat4 roll = glm::rotate(glm::mat4(1.0f), glm::radians(tiltOffset_), front_);
    glm::mat4 view = glm::lookAt(pos_, pos_ + front_, up_);
    return roll * view;
}

glm::mat4 Camera::GetProjectionMatrix(int w, int h) const {
    return glm::perspective(glm::radians(75.0f), (float)w / (float)h, 0.1f, 100.0f);
}

void Camera::UpdateVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    f.y = sin(glm::radians(pitch_));
    f.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(f);
    right_ = glm::normalize(glm::cross(front_, glm::vec3(0.0f, 1.0f, 0.0f)));
    up_    = glm::normalize(glm::cross(right_, front_));
}