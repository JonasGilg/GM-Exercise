#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif //_WIN32

#include <glm/vec3.hpp>

#ifdef _WIN32
    #include <optional>
#else
    #include <experimental/optional>
    using namespace std::experimental;
#endif

using namespace std;

struct Line final {
    const glm::vec3 start{};
    const glm::vec3 end{};
    const glm::vec3 direction{};
    const float magnitude;

    Line(const glm::vec3 &start, const glm::vec3 &end)
            : start(start),
              end(end),
              direction(end - start),
              magnitude(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z) {}

    optional <glm::vec3> intersect(const Line &other) {
        if (this->start == other.start || this->start == other.end || this->end == other.start ||
            this->end == other.end)
            return {};

        glm::vec3 u = this->direction;
        glm::vec3 v = other.direction;
        glm::vec3 w = this->start - other.start;

        float d = perp(u, v);

        float sI = perp(v, w) / d;
        if (sI < 0 || sI > 1)
            return {};

        float tI = perp(u, w) / d;
        if (tI < 0 || tI > 1)
            return {};

        return this->start + sI * u;
    }

    ~Line() = default;

private:
    inline float perp(const glm::vec3 &u, const glm::vec3 &v) {
        return u.x * v.y - u.y * v.x;
    }
};