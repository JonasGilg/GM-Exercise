#pragma once

#include <glm/vec3.hpp>

struct Line {
    const glm::vec3 start;
    const glm::vec3 end;

    glm::vec3 direction() const {
        return end - start;
    }

    float magnitude() const {
        glm::vec3 v = direction();
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }
};