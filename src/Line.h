#pragma once

#include <glm/vec3.hpp>

struct Line {
    const glm::vec3 start{};
    const glm::vec3 end{};
    const glm::vec3 direction{};
    const float magnitude;

    Line(const glm::vec3 &start, const glm::vec3 &end)
            : start(start),
              end(end),
              direction(end - start),
              magnitude(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z) {}

    virtual ~Line() = default;
};