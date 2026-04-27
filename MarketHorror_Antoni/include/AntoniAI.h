#pragma once
#include <glm/glm.hpp>
#include <string>

// Forward declarations
class Shader;
class ModelLoader;
class ParticleSystem;

enum class AntoniState {
    ABSENT,
    MATERIALIZING,
    INSPECTING,
    KIDNAPPING,
    IDLE_THREAT,
    THROWING_OIL,
    POINTING,
    CHASING,
    JUMPSCARE_POSE,
    CATCHING
};

class AntoniAI {
public:
    AntoniAI(const std::string& modelPath);
    ~AntoniAI();

    void Update(float dt, const glm::vec3& playerPos,
                float gameTime, ParticleSystem& particles);
    void Draw(const Shader& shader,
              const glm::mat4& view, const glm::mat4& proj);

    void Materialize(glm::vec3 position);
    void BeginKidnap(glm::vec3 victimPos);
    void BeginOilThrow(glm::vec3 centerPos);
    void BeginChase(glm::vec3 playerPos);
    void Reset();

    AntoniState State()       const { return state_; }
    glm::vec3   Position()    const { return pos_; }
    bool        CaughtPlayer(const glm::vec3& playerPos) const;
    bool        IsChasing()   const { return state_ == AntoniState::CHASING; }
    bool        IsJumpscare() const { return state_ == AntoniState::JUMPSCARE_POSE; }
    float       DistanceTo(const glm::vec3& p) const;

private:
    ModelLoader*  model_;
    AntoniState   state_       = AntoniState::ABSENT;
    glm::vec3     pos_;
    glm::vec3     targetPos_;
    glm::mat4     modelMatrix_;
    float         stateTimer_  = 0.0f;
    float         chaseSpeed_  = 5.5f;
    float         visibility_  = 0.0f;
    float         swayAngle_   = 0.0f;
    bool          hasOilJug_   = true;

    void UpdateChase(float dt, const glm::vec3& playerPos);
    void ComputeMatrix(float time);
    void TransitionTo(AntoniState s);
    glm::vec3 NavigateTo(const glm::vec3& target, float dt);
};
