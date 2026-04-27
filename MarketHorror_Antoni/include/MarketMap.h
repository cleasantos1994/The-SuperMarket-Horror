#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Collision.h"

class Shader;

struct MarketZone {
    std::string  name;
    glm::vec3    center;
    float        radius;
};

struct OilPuddle {
    glm::vec3  position;
    float      radius;
    bool       active;
    float      lifetime;
};

class MarketMap {
public:
    MarketMap();
    ~MarketMap();

    bool Init(const std::string& floorTexPath,
              const std::string& wallTexPath,
              const std::string& shelfTexPath);
    void Draw(const Shader& shader,
              const glm::mat4& view, const glm::mat4& proj,
              float time, float fearFactor);
    void Update(float dt);

    const std::vector<AABB>&       GetColliders()          const { return colliders_; }
    glm::vec3                      GetPlayerSpawn()        const { return {0, 0, 5}; }
    glm::vec3                      GetZoneCenter(const std::string& name) const;
    glm::vec3                      GetCarKeysSpawn()       const;
    glm::vec3                      GetExitDoorPos()        const;
    glm::vec3                      GetCarPos()             const;
    glm::vec3                      GetAntoniSpawnPos()     const;
    glm::vec3                      GetBackRoomDoorPos()    const;

    void AddOilPuddle(glm::vec3 pos, float radius = 1.5f);
    bool PlayerInOilPuddle(const glm::vec3& playerPos) const;
    void DropPoopDecal(glm::vec3 pos);
    void OpenExitDoor();

    const std::vector<MarketZone>& GetZones() const { return zones_; }

private:
    unsigned int floorTexID_, wallTexID_, shelfTexID_, poopDecalTexID_, oilTexID_;
    unsigned int floorVAO_, wallVAO_, shelfVAO_;
    std::vector<AABB>        colliders_;
    std::vector<MarketZone>  zones_;
    std::vector<OilPuddle>   oilPuddles_;
    bool                     exitDoorOpen_ = false;
    float                    exitDoorAngle_= 0.0f;

    void BuildFloor();
    void BuildWalls();
    void BuildShelves();
    void BuildCheckout();
    void BuildBackRoom();
    void BuildParking();
    void UploadGeometry();
    unsigned int LoadTex(const std::string& path);
};
