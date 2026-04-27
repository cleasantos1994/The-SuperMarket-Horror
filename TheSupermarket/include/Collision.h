#pragma once
#include <glm/glm.hpp>
#include <algorithm>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    static glm::vec3 Resolve(const AABB& a, const AABB& b) {
        float dx1 = b.max.x - a.min.x;
        float dx2 = a.max.x - b.min.x;
        float dy1 = b.max.y - a.min.y;
        float dy2 = a.max.y - b.min.y;
        float dz1 = b.max.z - a.min.z;
        float dz2 = a.max.z - b.min.z;

        float minX = std::min(dx1, dx2);
        float minY = std::min(dy1, dy2);
        float minZ = std::min(dz1, dz2);

        if (minX < minY && minX < minZ) {
            return (dx1 < dx2) ? glm::vec3(dx1, 0, 0) : glm::vec3(-dx2, 0, 0);
        } else if (minY < minX && minY < minZ) {
            return (dy1 < dy2) ? glm::vec3(0, dy1, 0) : glm::vec3(0, -dy2, 0);
        } else {
            return (dz1 < dz2) ? glm::vec3(0, 0, dz1) : glm::vec3(0, 0, -dz2);
        }
    }
};

inline AABB MakeAABB(glm::vec3 center, glm::vec3 halfSize) {
    return { center - halfSize, center + halfSize };
}
