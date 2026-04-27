#include "MarketMap.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <ctime>

MarketMap::MarketMap() {
    std::srand((unsigned int)std::time(nullptr));
    billboardNumber_ = std::rand() % 900 + 100; // 100-999

    zones_.push_back({"CHECKOUT", {0, 0, 0}, 5.0f});
    zones_.push_back({"AISLE_A", {5, 0, -5}, 3.0f});
    zones_.push_back({"AISLE_C", {-5, 0, -5}, 3.0f});
    zones_.push_back({"EXIT", {0, 0, 15}, 4.0f});
    zones_.push_back({"FROZEN", {-10, 0, 5}, 4.0f});
    zones_.push_back({"TV", {8, 1, 2}, 2.0f});
}

MarketMap::~MarketMap() {
    glDeleteVertexArrays(1, &floorVAO_);
    glDeleteVertexArrays(1, &wallVAO_);
    glDeleteVertexArrays(1, &shelfVAO_);
    glDeleteVertexArrays(1, &frozenVAO_);
    glDeleteVertexArrays(1, &parkingVAO_);
    glDeleteVertexArrays(1, &billboardVAO_);
}

bool MarketMap::Init(const std::string& floorTexPath, const std::string& wallTexPath, const std::string& shelfTexPath) {
    BuildFloor();
    BuildWalls();
    BuildShelves();
    UploadGeometry();
    return true;
}

void MarketMap::Draw(const Shader& shader, const glm::mat4& view, const glm::mat4& proj, float time, float fearFactor) {
    shader.Use();
    shader.SetMat4("view", view);
    shader.SetMat4("projection", proj);
    shader.SetMat4("model", glm::mat4(1.0f));
    shader.SetBool("lightsOut", lightsOut_);
    
    glBindVertexArray(floorVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(shelfVAO_);
    glDrawArrays(GL_TRIANGLES, 0, (int)colliders_.size() * 36);
}

void MarketMap::Update(float dt) {
    if (exitDoorOpen_ && exitDoorAngle_ < 90.0f) {
        exitDoorAngle_ += dt * 45.0f;
    }
    if (secretDoorOpen_ && secretDoorAngle_ < 90.0f) {
        secretDoorAngle_ += dt * 45.0f;
    }
}

void MarketMap::AddOilPuddle(glm::vec3 pos, float radius) {
    oilPuddles_.push_back({pos, radius, true, 999.0f});
}

bool MarketMap::PlayerInOilPuddle(const glm::vec3& playerPos) const {
    for (auto& p : oilPuddles_) {
        if (p.active && glm::distance(playerPos, p.position) < p.radius) return true;
    }
    return false;
}

void MarketMap::DropPoopDecal(glm::vec3 pos) {}
void MarketMap::OpenExitDoor() { exitDoorOpen_ = true; }
void MarketMap::OpenSecretDoor() { secretDoorOpen_ = true; }
void MarketMap::SetLightsOut(bool off) { lightsOut_ = off; }

glm::vec3 MarketMap::GetZoneCenter(const std::string& name) const {
    for (auto& z : zones_) if (z.name == name) return z.center;
    return {0,0,0};
}

glm::vec3 MarketMap::GetCarKeysSpawn() const { return {-8, 0.5f, -10}; }
glm::vec3 MarketMap::GetExitDoorPos() const { return {0, 0, 20}; }
glm::vec3 MarketMap::GetCarPos() const { return {0, 0, 25}; }
glm::vec3 MarketMap::GetAntoniSpawnPos() const { return {10, 0, -10}; }
glm::vec3 MarketMap::GetBackRoomDoorPos() const { return {0, 0, -15}; }
glm::vec3 MarketMap::GetFrozenSectionPos() const { return {-10, 0, 5}; }
glm::vec3 MarketMap::GetTVPos() const { return {8, 1, 2}; }
glm::vec3 MarketMap::GetSecretDoorPos() const { return {-12, 0, 5}; }
glm::vec3 MarketMap::GetBillboardPos() const { return {20, 5, 30}; }

void MarketMap::BuildFloor() {}
void MarketMap::BuildWalls() {}
void MarketMap::BuildShelves() {
    colliders_.push_back(MakeAABB({5, 1, -5}, {1, 1, 5}));
}
void MarketMap::UploadGeometry() {
    glGenVertexArrays(1, &floorVAO_);
    glGenVertexArrays(1, &shelfVAO_);
    glGenVertexArrays(1, &wallVAO_);
}
unsigned int MarketMap::LoadTex(const std::string& path) { return 0; }
