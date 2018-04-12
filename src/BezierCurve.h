#pragma once

#ifdef _WIN32

#include <windows.h>

#endif //_WIN32

#include <vector>
#include <tuple>
#include <GL/gl.h>
#include <glm/vec3.hpp>


using PointList = std::vector<glm::vec3>;

struct BezierCurve {
    BezierCurve(const PointList &controlPoints,
                glm::vec3 pointColor,
                glm::vec3 meshColor,
                glm::vec3 curveColor);

    void setPicked(int i, glm::vec3 picked);

    void draw() const;
    void drawPoints(GLenum mode) const;

    PointList intersects(const BezierCurve &other) const;

    virtual ~BezierCurve() = default;

    const unsigned long offset;
    const unsigned long offsetEnd;

private:
    void update();

    void plotBezier(const PointList &currPoints);
    std::pair<PointList, PointList> deCasteljau(const PointList &currPoints) const;

    PointList intersectsRecursive(const PointList &curve1, const PointList &curve2) const;
    PointList recurse(const PointList &curve1, const PointList &curve2) const;

    void drawMesh() const;
    void drawCurve() const;

    PointList controlPoints;
    PointList curvePoints;

    const glm::vec3 pointColor;
    const glm::vec3 meshColor;
    const glm::vec3 curveColor;

    static unsigned long offsetCounter;

};