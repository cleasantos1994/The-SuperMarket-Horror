#include "AntoniAI.h"
#include "ModelLoader.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

AntoniAI::AntoniAI(const std::string& modelPath) {
    model_ = new ModelLoader(modelPath);
    pos_ = {0, 0, 0};
}

AntoniAI::~AntoniAI() {
    delete model_;
}

void AntoniAI::Update(float dt, const glm::vec3& playerPos, float gameTime, ParticleSystem& particles) {
    stateTimer_ += dt;
    switch (state_) {
        case AntoniState::CHASING:
            UpdateChase(dt, playerPos);
            break;
        case AntoniState::DISGUISED:
            // Just stand still like a customer
            break;
        case AntoniState::AMBUSHING:
            if (glm::distance(pos_, playerPos) < 3.0f) {
                TransitionTo(AntoniState::CHASING);
            }
            break;
        default: break;
    }
    ComputeMatrix(gameTime);
}

void AntoniAI::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
    if (state_ == AntoniState::ABSENT) return;
    shader.Use();
    shader.SetMat4("view", view);
    shader.SetMat4("projection", proj);
    shader.SetMat4("model", modelMatrix_);
    shader.SetFloat("visibility", visibility_);
    model_->Draw(shader);
}

void AntoniAI::Materialize(glm::vec3 position) {
    pos_ = position;
    visibility_ = 1.0f;
    TransitionTo(AntoniState::MATERIALIZING);
}

void AntoniAI::BeginKidnap(glm::vec3 victimPos) {
    targetPos_ = victimPos;
    TransitionTo(AntoniState::KIDNAPPING);
}

void AntoniAI::BeginOilThrow(glm::vec3 centerPos) {
    pos_ = centerPos;
    TransitionTo(AntoniState::THROWING_OIL);
}

void AntoniAI::BeginChase(glm::vec3 playerPos) {
    TransitionTo(AntoniState::CHASING);
}

void AntoniAI::SetDisguised(bool disguised) {
    isDisguised_ = disguised;
    if (disguised) TransitionTo(AntoniState::DISGUISED);
}

void AntoniAI::SetAmbush(bool ambush, glm::vec3 pos) {
    if (ambush) {
        pos_ = pos;
        TransitionTo(AntoniState::AMBUSHING);
    }
}

void AntoniAI::Reset() {
    state_ = AntoniState::ABSENT;
    visibility_ = 0.0f;
}

bool AntoniAI::CaughtPlayer(const glm::vec3& playerPos) const {
    return state_ == AntoniState::CHASING && glm::distance(pos_, playerPos) < 1.0f;
}

float AntoniAI::DistanceTo(const glm::vec3& p) const {
    return glm::distance(pos_, p);
}

void AntoniAI::UpdateChase(float dt, const glm::vec3& playerPos) {
    glm::vec3 dir = glm::normalize(playerPos - pos_);
    pos_ += dir * chaseSpeed_ * dt;
}

void AntoniAI::ComputeMatrix(float time) {
    modelMatrix_ = glm::translate(glm::mat4(1.0f), pos_);
}

void AntoniAI::TransitionTo(AntoniState s) {
    state_ = s;
    stateTimer_ = 0.0f;
}

glm::vec3 AntoniAI::NavigateTo(const glm::vec3& target, float dt) {
    return glm::normalize(target - pos_) * dt;
}
