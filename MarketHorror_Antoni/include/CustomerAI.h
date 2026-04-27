#pragma once
#include <glm/glm.hpp>
#include <string>

// Forward declarations
class Shader;
class ModelLoader;

enum class CustomerType {
    NORMAL,
    STRANGE,
    CALEB,
    WILLIAM,
    GUILHERME
};

enum class CustomerState {
    ENTERING,
    WANDERING,
    NEEDS_HELP,
    PRE_INCIDENT,
    INCIDENT,
    BEING_KIDNAPPED,
    GONE
};

class CustomerAI {
public:
    CustomerAI(CustomerType type, glm::vec3 spawnPos,
               const std::string& modelPath);
    ~CustomerAI();

    void Update(float dt, const glm::vec3& playerPos, float gameTime);
    void Draw(const Shader& shader,
              const glm::mat4& view, const glm::mat4& proj);
    void TriggerIncident();
    void BeginKidnap();

    CustomerType   Type()   const { return type_; }
    CustomerState  State()  const { return state_; }
    glm::vec3      Pos()    const { return pos_; }
    bool           HasPooped() const { return hasPooped_; }

private:
    CustomerType   type_;
    CustomerState  state_;
    ModelLoader*   model_;
    glm::vec3      pos_;
    glm::vec3      targetPos_;
    glm::mat4      modelMatrix_;
    bool           hasPooped_      = false;
    float          stateTimer_     = 0.0f;
    float          walkSpeed_      = 1.2f;
    float          squatPhase_     = 0.0f;

    void UpdateNormal(float dt, const glm::vec3& playerPos);
    void UpdateStrange(float dt, float gameTime);
    void UpdateCoworker(float dt, const glm::vec3& playerPos); // For Caleb, William, Guilherme
    void ComputeMatrix(float time);
};
