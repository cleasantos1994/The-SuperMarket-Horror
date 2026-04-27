#include "CustomerAI.h"
#include "ModelLoader.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

CustomerAI::CustomerAI(CustomerType type, glm::vec3 spawnPos, const std::string& modelPath)
    : type_(type), pos_(spawnPos) {
    model_ = new ModelLoader(modelPath);
    state_ = CustomerState::WANDERING;
}

CustomerAI::~CustomerAI() {
    delete model_;
}

void CustomerAI::Update(float dt, const glm::vec3& playerPos, float gameTime) {
    stateTimer_ += dt;
    switch (type_) {
        case CustomerType::STRANGE:
            UpdateStrange(dt, gameTime);
            break;
        case CustomerType::NORMAL:
            UpdateNormal(dt, playerPos);
            break;
        case CustomerType::CALEB:
        case CustomerType::WILLIAM:
        case CustomerType::GUILHERME:
            UpdateCoworker(dt, playerPos);
            break;
        default: break;
    }
    ComputeMatrix(gameTime);
}

void CustomerAI::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
    if (state_ == CustomerState::GONE) return;
    shader.Use();
    shader.SetMat4("model", modelMatrix_);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", proj);
    model_->Draw(shader);
}

void CustomerAI::TriggerIncident() {
    state_ = CustomerState::INCIDENT;
    stateTimer_ = 0;
}

void CustomerAI::BeginKidnap() {
    state_ = CustomerState::BEING_KIDNAPPED;
    stateTimer_ = 0;
}

void CustomerAI::UpdateNormal(float dt, const glm::vec3& playerPos) {
    // Basic idling
}

void CustomerAI::UpdateStrange(float dt, float gameTime) {
    if (state_ == CustomerState::INCIDENT) {
        if (stateTimer_ > 2.0f) hasPooped_ = true;
    }
    if (state_ == CustomerState::BEING_KIDNAPPED) {
        pos_ += glm::vec3(0, 0, -1) * dt * 2.0f;
        if (stateTimer_ > 3.0f) state_ = CustomerState::GONE;
    }
}

void CustomerAI::UpdateCoworker(float dt, const glm::vec3& playerPos) {
    // Look at player if close
}

void CustomerAI::ComputeMatrix(float time) {
    modelMatrix_ = glm::translate(glm::mat4(1.0f), pos_);
    if (state_ == CustomerState::INCIDENT) {
        float squat = std::abs(std::sin(time * 5.0f)) * 0.3f;
        modelMatrix_ = glm::translate(modelMatrix_, {0, -squat, 0});
    }
    modelMatrix_ = glm::scale(modelMatrix_, glm::vec3(0.9f));
}