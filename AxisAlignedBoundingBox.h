#pragma once

#include <glm/vec3.hpp>
#include <vector>

using namespace glm;
using namespace std;

struct AxisAlignedBoundingBox {
    const vec3 min;
    const vec3 max;

    bool intersects(const AxisAlignedBoundingBox &other) const;

    static AxisAlignedBoundingBox createFromMesh(const vector<vec3> &mesh);
};

typedef AxisAlignedBoundingBox AABB;
