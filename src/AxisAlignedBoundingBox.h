#pragma once

#ifdef _WIN32
#   include <windows.h>
#endif //_WIN32

#include <vector>
#include <glm/vec3.hpp>

struct AxisAlignedBoundingBox final {
    const glm::vec3 min;
    const glm::vec3 max;

    bool intersects(const AxisAlignedBoundingBox &other) const;

    static AxisAlignedBoundingBox createFromMesh(const std::vector<glm::vec3> &mesh);
};

typedef AxisAlignedBoundingBox AABB;