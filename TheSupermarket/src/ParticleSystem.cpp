#include "ParticleSystem.h"
#include "Shader.h"
#include <algorithm>
#include <cstdlib>

ParticleSystem::ParticleSystem(int maxParticles) : max_(maxParticles) {
    particles_.resize(max_);
    for (auto& p : particles_) p.active = false;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    // Particle data: position (3) + color (4) + size (1) = 8 floats
    glBufferData(GL_ARRAY_BUFFER, max_ * 8 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(7 * sizeof(float)));
    glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void ParticleSystem::Update(float dt) {
    for (auto& p : particles_) {
        if (!p.active) continue;
        p.life -= dt;
        if (p.life <= 0) { p.active = false; continue; }
        p.pos += p.vel * dt;
        p.color.a = p.life / p.maxLife;
    }
}

void ParticleSystem::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camRight, const glm::vec3& camUp) {
    std::vector<float> data;
    for (auto& p : particles_) {
        if (!p.active) continue;
        data.push_back(p.pos.x); data.push_back(p.pos.y); data.push_back(p.pos.z);
        data.push_back(p.color.r); data.push_back(p.color.g); data.push_back(p.color.b); data.push_back(p.color.a);
        data.push_back(p.size);
    }
    if (data.empty()) return;

    shader.Use();
    shader.SetMat4("view", view);
    shader.SetMat4("projection", proj);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), &data[0]);
    glBindVertexArray(VAO_);
    glDrawArrays(GL_POINTS, 0, data.size() / 8);
    glBindVertexArray(0);
}

void ParticleSystem::EmitPoopStorm(glm::vec3 origin, int count) {
    for (int i = 0; i < count; i++) {
        int idx = FindDead();
        if (idx == -1) break;
        particles_[idx].active = true;
        particles_[idx].pos = origin;
        particles_[idx].vel = glm::vec3((rand()%100-50)/20.0f, (rand()%100)/10.0f, (rand()%100-50)/20.0f);
        particles_[idx].color = glm::vec4(0.36f, 0.23f, 0.12f, 1.0f);
        particles_[idx].life = particles_[idx].maxLife = 2.0f + (rand()%100)/100.0f;
        particles_[idx].size = 0.1f + (rand()%100)/500.0f;
    }
}

void ParticleSystem::EmitBabyOilSplash(glm::vec3 origin, int count) {
    for (int i = 0; i < count; i++) {
        int idx = FindDead();
        if (idx == -1) break;
        particles_[idx].active = true;
        particles_[idx].pos = origin;
        particles_[idx].vel = glm::vec3((rand()%100-50)/15.0f, (rand()%100)/8.0f, (rand()%100-50)/15.0f);
        particles_[idx].color = glm::vec4(1.0f, 1.0f, 0.9f, 0.6f);
        particles_[idx].life = particles_[idx].maxLife = 1.0f + (rand()%100)/100.0f;
        particles_[idx].size = 0.05f + (rand()%100)/1000.0f;
    }
}

int ParticleSystem::FindDead() {
    for (int i = 0; i < max_; i++) if (!particles_[i].active) return i;
    return -1;
}

void ParticleSystem::EmitPoopRegister(glm::vec3 origin, int count) { EmitPoopStorm(origin, count); }
void ParticleSystem::EmitOilDrip(glm::vec3 origin) { EmitBabyOilSplash(origin, 1); }
void ParticleSystem::EmitKeySparkle(glm::vec3 origin) {}
void ParticleSystem::EmitFear(glm::vec3 origin, int count) {}
void ParticleSystem::EmitAntoniMaterialize(glm::vec3 origin) {}