#include "AxisAlignedBoundingBox.h"

bool AxisAlignedBoundingBox::intersects(const AxisAlignedBoundingBox &other) const {
    return (this->min.x <= other.max.x && this->max.x >= other.min.x) &&
           (this->min.y <= other.max.y && this->max.y >= other.min.y) &&
           (this->min.z <= other.max.z && this->max.z >= other.min.z);
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::createFromMesh(const std::vector<glm::vec3> &mesh) {
    glm::vec3 min = mesh[0];
    glm::vec3 max = mesh[0];

    for (auto &&point : mesh) {
        if (point.x < min.x) min.x = point.x;
        if (point.y < min.y) min.y = point.y;
        if (point.z < min.z) min.z = point.z;

        if (point.x > max.x) max.x = point.x;
        if (point.y > max.y) max.y = point.y;
        if (point.z > max.z) max.z = point.z;
    }

    return {min, max};
}
