#include "CustomerAI.h"
#include "ModelLoader.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

CustomerAI::CustomerAI(CustomerType type, glm::vec3 spawnPos, const std::string& modelPath) {
    type_ = type;
    pos_ = spawnPos;
    model_ = new ModelLoader(modelPath);
    state_ = CustomerState::WANDERING;
}

CustomerAI::~CustomerAI() {
    delete model_;
}

void CustomerAI::Update(float dt, const glm::vec3& playerPos, float gameTime) {
    if (state_ == CustomerState::GONE) return;
    
    if (isGhostly_) {
        // Ghosts just fade in and out or wander randomly
        pos_.y = 0.5f + sin(gameTime * 2.0f) * 0.2f;
    }
    
    switch (type_) {
        case CustomerType::STRANGE:
            UpdateStrange(dt, gameTime);
            break;
        case CustomerType::CALEB:
        case CustomerType::WILLIAM:
        case CustomerType::GUILHERME:
            UpdateCoworker(dt, playerPos);
            break;
        default:
            UpdateNormal(dt, playerPos);
            break;
    }
    ComputeMatrix(gameTime);
}

void CustomerAI::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
    if (state_ == CustomerState::GONE) return;
    shader.Use();
    shader.SetMat4("view", view);
    shader.SetMat4("projection", proj);
    shader.SetMat4("model", modelMatrix_);
    shader.SetBool("isGhostly", isGhostly_);
    model_->Draw(shader);
}

void CustomerAI::TriggerIncident() {
    state_ = CustomerState::INCIDENT;
    hasPooped_ = true;
}

void CustomerAI::BeginKidnap() {
    state_ = CustomerState::BEING_KIDNAPPED;
}

void CustomerAI::SetGhostly(bool ghostly) {
    isGhostly_ = ghostly;
    if (ghostly) state_ = CustomerState::GHOSTLY;
}

void CustomerAI::UpdateNormal(float dt, const glm::vec3& playerPos) {}
void CustomerAI::UpdateStrange(float dt, float gameTime) {}
void CustomerAI::UpdateCoworker(float dt, const glm::vec3& playerPos) {}

void CustomerAI::ComputeMatrix(float time) {
    modelMatrix_ = glm::translate(glm::mat4(1.0f), pos_);
}
