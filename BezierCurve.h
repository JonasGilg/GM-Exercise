#pragma once

#ifdef _WIN32

#include <windows.h>

#endif //_WIN32

#include <vector>
#include <tuple>
#include <GL/gl.h>
#include <glm/vec3.hpp>

struct BezierCurve {
    BezierCurve(const std::vector<glm::vec3> &controlPoints,
                glm::vec3 pointColor,
                glm::vec3 meshColor,
                glm::vec3 curveColor);

    void setPicked(int i, glm::vec3 picked);

    void draw() const;
    void drawPoints(GLenum mode) const;

    std::vector<glm::vec3> intersects(const BezierCurve &other) const;

    virtual ~BezierCurve() = default;

    const unsigned long offset;
    const unsigned long offsetEnd;

private:
    void update();

    void plotBezier(const std::vector<glm::vec3> &currPoints);
    std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> deCasteljau(const std::vector<glm::vec3> &currPoints) const;

    std::vector<glm::vec3> intersectsRecursive(const std::vector<glm::vec3> &v1, const std::vector<glm::vec3> &v2) const;

    void drawMesh() const;
    void drawCurve() const;

    std::vector<glm::vec3> controlPoints;
    std::vector<glm::vec3> curvePoints;

    const glm::vec3 pointColor;
    const glm::vec3 meshColor;
    const glm::vec3 curveColor;

    static unsigned long offsetCounter;
};