#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Export.h"

class GAMECORE_API Camera {
public:
    Camera(glm::vec3 startPos = {0.f, 1.7f, 0.f},
           float yaw = -90.f, float pitch = 0.f);

    void Update(float dx, float dy, float sensitivity);
    void Move(glm::vec3 direction, float speed, float dt);
    void Bob(bool isMoving, float dt);
    void ApplyFearEffect(int fear, float time);
    void ApplySlip(float dt);
    void StopSlip();

    glm::mat4 GetViewMatrix()                     const;
    glm::mat4 GetProjectionMatrix(int w, int h)   const;
    glm::vec3 Position()  const { return pos_; }
    glm::vec3 Front()     const { return front_; }
    glm::vec3 Right()     const { return right_; }
    bool      IsSlipping() const { return slipping_; }

private:
    glm::vec3 pos_, front_, up_, right_;
    float     yaw_, pitch_;
    float     bobPhase_    = 0.0f;
    float     tiltOffset_  = 0.0f;
    float     slipVelX_    = 0.0f;
    float     slipVelZ_    = 0.0f;
    bool      slipping_    = false;
    void      UpdateVectors();
};
