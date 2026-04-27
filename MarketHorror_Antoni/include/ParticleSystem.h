#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;

struct Particle {
    glm::vec3 pos, vel;
    glm::vec4 color;
    float     life, maxLife, size;
    bool      active;
};

class ParticleSystem {
public:
    explicit ParticleSystem(int maxParticles = 512);
    ~ParticleSystem();

    void Update(float dt);
    void Draw(const Shader& shader, const glm::mat4& view,
              const glm::mat4& proj,
              const glm::vec3& camRight, const glm::vec3& camUp);

    void EmitPoopStorm(glm::vec3 origin, int count = 64);
    void EmitPoopRegister(glm::vec3 origin, int count = 80);
    void EmitBabyOilSplash(glm::vec3 origin, int count = 48);
    void EmitOilDrip(glm::vec3 origin);
    void EmitKeySparkle(glm::vec3 origin);
    void EmitFear(glm::vec3 origin, int count = 12);
    void EmitAntoniMaterialize(glm::vec3 origin);

private:
    std::vector<Particle> particles_;
    unsigned int VAO_, VBO_;
    int          max_;
    unsigned int brownTexID_, oilTexID_, sparkTexID_, smokeTexID_;
    int FindDead();
};
