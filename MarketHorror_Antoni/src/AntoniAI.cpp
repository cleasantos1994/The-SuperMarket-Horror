#include "AntoniAI.h"
#include "ModelLoader.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

AntoniAI::AntoniAI(const std::string& modelPath) {
    model_ = new ModelLoader(modelPath);
    pos_ = {0, 0, 0};
    targetPos_ = {0, 0, 0};
    TransitionTo(AntoniState::ABSENT);
}

AntoniAI::~AntoniAI() {
    delete model_;
}

void AntoniAI::Update(float dt, const glm::vec3& playerPos, float gameTime, ParticleSystem& particles) {
    if (state_ == AntoniState::ABSENT) return;
    
    stateTimer_ += dt;
    
    switch (state_) {
        case AntoniState::MATERIALIZING:
            visibility_ = std::min(1.0f, stateTimer_ / 2.0f);
            if (stateTimer_ >= 2.0f) TransitionTo(AntoniState::INSPECTING);
            break;
        case AntoniState::INSPECTING:
            if (stateTimer_ >= 3.0f) TransitionTo(AntoniState::IDLE_THREAT);
            break;
        case AntoniState::CHASING:
            UpdateChase(dt, playerPos);
            break;
        case AntoniState::KIDNAPPING:
            pos_ += glm::vec3(0, 0, -1) * dt * 2.0f;
            if (stateTimer_ > 3.0f) TransitionTo(AntoniState::ABSENT);
            break;
        default: break;
    }
    
    ComputeMatrix(gameTime);
}

void AntoniAI::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
    if (state_ == AntoniState::ABSENT) return;
    shader.Use();
    shader.SetMat4("model", modelMatrix_);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", proj);
    shader.SetFloat("visibility", visibility_);
    model_->Draw(shader);
}

void AntoniAI::Materialize(glm::vec3 position) {
    pos_ = position;
    stateTimer_ = 0;
    TransitionTo(AntoniState::MATERIALIZING);
}

void AntoniAI::BeginKidnap(glm::vec3 victimPos) {
    pos_ = victimPos + glm::vec3(0, 0, 0.5f);
    TransitionTo(AntoniState::KIDNAPPING);
}

void AntoniAI::BeginChase(glm::vec3 playerPos) {
    TransitionTo(AntoniState::CHASING);
}

void AntoniAI::Reset() {
    TransitionTo(AntoniState::ABSENT);
}

bool AntoniAI::CaughtPlayer(const glm::vec3& playerPos) const {
    return state_ == AntoniState::CHASING && glm::distance(pos_, playerPos) < 1.2f;
}

float AntoniAI::DistanceTo(const glm::vec3& p) const {
    return glm::distance(pos_, p);
}

void AntoniAI::UpdateChase(float dt, const glm::vec3& playerPos) {
    glm::vec3 dir = playerPos - pos_;
    dir.y = 0;
    if (glm::length(dir) > 0.1f) {
        dir = glm::normalize(dir);
        pos_ += dir * chaseSpeed_ * dt;
    }
}

void AntoniAI::ComputeMatrix(float time) {
    modelMatrix_ = glm::translate(glm::mat4(1.0f), pos_);
    modelMatrix_ = glm::scale(modelMatrix_, glm::vec3(1.2f, 1.4f, 1.2f));
}

void AntoniAI::TransitionTo(AntoniState s) {
    state_ = s;
    stateTimer_ = 0;
}

glm::vec3 AntoniAI::NavigateTo(const glm::vec3& target, float dt) {
    return glm::normalize(target - pos_) * dt;
}