#pragma once

#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

#include <vector>
#include <GL/gl.h>
#include <glm/vec3.hpp>

struct BezierCurve;

using PointList = std::vector<glm::vec3>;

struct BSplineCurve final {
    BSplineCurve(const PointList &controlPoints,
                const glm::vec3 &pointColor,
                const glm::vec3 &meshColor,
                const glm::vec3 &curveColor);

    void setPicked(int i, const glm::vec3 &picked);

    void draw() const;
    void drawPoints(GLenum mode) const;

    ~BSplineCurve() = default;

    const unsigned long offset;
    const unsigned long offsetEnd;

private:
    void update();
    void calculateBezier();
    void deBoor();
    void deBoorRecursive();

    void drawMesh() const;

    std::vector<BezierCurve> bezierCurves;
    PointList controlPoints;

    const glm::vec3 pointColor;
    const glm::vec3 meshColor;
    const glm::vec3 curveColor;

    static unsigned long offsetCounter;
};