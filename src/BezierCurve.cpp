#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <GL/glut.h>
#include <algorithm>

#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "BezierCurve.h"
#include "AxisAlignedBoundingBox.h"
#include "Line.h"

#if _WIN32
    #include <optional>
#else
    #include <experimental/optional>
    using namespace std::experimental;
#endif

using PointList = std::vector<glm::vec3>;

using namespace std;
using namespace glm;

constexpr float EPSILON = 0.001f;

unsigned long BezierCurve::offsetCounter = 0;

BezierCurve::BezierCurve(const PointList &controlPoints,
                         const vec3 &pointColor,
                         const vec3 &meshColor,
                         const vec3 &curveColor)
        : controlPoints(controlPoints),
          pointColor(pointColor),
          meshColor(meshColor),
          curveColor(curveColor),
          offset(offsetCounter),
          offsetEnd(static_cast<const unsigned long>(offsetCounter + controlPoints.size())) {
    update();
    offsetCounter = offsetEnd;
}

void BezierCurve::update() {
    curvePoints.clear();
    plotBezier(controlPoints);
    intersectsSelf();
}

void BezierCurve::setPicked(int i, const vec3 &picked) {
    controlPoints[i - offset] = picked;
    update();
}

bool BezierCurve::isFlat(const PointList &mesh) const {
    for (int i = 1; i < mesh.size() - 1; ++i) {
        if (Line(mesh[i + 1] - mesh[i], mesh[i] - mesh[i - 1]).magnitude > EPSILON)
            return false;
    }
    return true;
}

void BezierCurve::recurse(const PointList &curve1, const PointList &curve2, PointList &resultList) const {
    auto result = deCasteljau(curve1);
    PointList curve1A = result.first;
    PointList curve1B = result.second;

    intersectsRecursive(curve1A, curve2, resultList);
    intersectsRecursive(curve1B, curve2, resultList);
}

void BezierCurve::intersectsRecursive(const PointList &curve1, const PointList &curve2, PointList &resultList) const {
    auto curve1BB = AABB::createFromMesh(curve1);
    auto curve2BB = AABB::createFromMesh(curve2);

    if (curve1BB.intersects(curve2BB)) {
        if (!isFlat(curve1)) {
            recurse(curve1, curve2, resultList);
        } else if (!isFlat(curve2)) {
            recurse(curve2, curve1, resultList);
        } else {
            auto result = Line{curve1.front(), curve1.back()}.intersect({curve2.front(), curve2.back()});
            if (result)
                resultList.push_back(*result);
        }
    }
}

PointList BezierCurve::intersects(const BezierCurve &other) const {
    PointList resultList;
    intersectsRecursive(this->controlPoints, other.controlPoints, resultList);
    return resultList;
}

void BezierCurve::draw() const {
    drawCurve();
    drawMesh();
    drawPoints(GL_RENDER);

    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (auto &&point : selfIntersections) {
        glVertex3fv(value_ptr(point));
    }
    glEnd();
}

void BezierCurve::drawPoints(GLenum mode) const {
    glColor3fv(value_ptr(pointColor));

    for (int i = 0; i < controlPoints.size(); i++) {
        if (mode == GL_SELECT)
            glLoadName(static_cast<GLuint>(i + offset));

        glBegin(GL_POINTS);
        glVertex3fv(value_ptr(controlPoints[i]));
        glEnd();
    }
}

void BezierCurve::drawMesh() const {
    glColor3fv(value_ptr(meshColor));
    glBegin(GL_LINE_STRIP);
    for (auto &&point : controlPoints) {
        glVertex3fv(value_ptr(point));
    }
    glEnd();
}

void BezierCurve::drawCurve() const {
    glColor3fv(value_ptr(curveColor));
    glBegin(GL_LINE_STRIP);
    for (auto &&point : curvePoints) {
        glVertex3fv(value_ptr(point));
    }
    glEnd();
}

pair<PointList, PointList> BezierCurve::deCasteljau(const PointList &currPoints) const {
    size_t n = currPoints.size();
    vector<PointList> values(n, PointList(n));

    for (int x = 0; x < n; ++x) {
        for (int y = x; y < n; ++y) {
            if (x == 0) {
                values[x][y] = currPoints[y];
            } else {
                vec3 first = values[x - 1][y];
                vec3 second = values[x - 1][y - 1];
                values[x][y] = (first + second) * 0.5f;
            }
        }
    }

    PointList p1;
    PointList p2;

    for (int i = 0; i < n; ++i) {
        p1.push_back(values[i][i]);
        p2.push_back(values[n - i - 1][n - 1]);
    }

    return {p1, p2};
}

void BezierCurve::plotBezier(const PointList &currPoints) {
    if (isFlat(currPoints)) {
        curvePoints.insert(curvePoints.end(), currPoints.begin(), currPoints.end());
    } else {
        auto c = deCasteljau(currPoints);
        plotBezier(c.first);
        plotBezier(c.second);
    }
}

float angleBetweenVectors(const vec3 &v1, const vec3 &v2) {
    float angle = acos(dot(normalize(v1), normalize(v2)));
    auto c = cross(v1, v2);
    if (dot({0, 0, 1}, c) < 0) {
        angle *= -1;
    }
    return angle;
}

bool canSelfIntersect(const PointList &points) {
    for (int i = 0; i < points.size() - 1; ++i) {

        float angle = 0.0;
        for (int j = i; j < points.size() - 1; ++j) {
            angle += angleBetweenVectors(points[j], points[j + 1]);
            if (angle > M_PI || angle < -M_PI)
                return true;
        }
    }

    return false;
}

PointList derive(const PointList &points) {
    size_t n = points.size();
    PointList derivative;

    for (int i = 0; i < n - 1; ++i) {
        derivative.push_back((float) n * (points[i + 1] - points[i]));
    }

    return derivative;
}

void BezierCurve::selfIntersectsRecursive(const PointList &currPoints) {
    if (canSelfIntersect(derive(currPoints))) {
        auto result = deCasteljau(currPoints);
        selfIntersectsRecursive(result.first);
        selfIntersectsRecursive(result.second);
    } else {
        selfIntersectionParts.push_back(currPoints);
    }
}

void BezierCurve::intersectsSelf() {
    selfIntersections.clear();
    selfIntersectionParts.clear();
    selfIntersectsRecursive(controlPoints);

    for (int i = 0; i < selfIntersectionParts.size(); ++i) {
        for (int j = i + 1; j < selfIntersectionParts.size(); ++j) {
            auto list1 = selfIntersectionParts[i];
            auto list2 = selfIntersectionParts[j];

            intersectsRecursive(list1, list2, selfIntersections);
        }
    }
}
