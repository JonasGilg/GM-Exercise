#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "BezierCurve.h"
#include "AxisAlignedBoundingBox.h"
#include "Line.h"

using PointList = std::vector<glm::vec3>;

using namespace std;
using namespace experimental;
using namespace glm;

constexpr float EPSILON = 0.001f;

unsigned long BezierCurve::offsetCounter = 0;

BezierCurve::BezierCurve(const PointList &controlPoints,
                         vec3 pointColor,
                         vec3 meshColor,
                         vec3 curveColor)
        : controlPoints(controlPoints),
          pointColor(pointColor),
          meshColor(meshColor),
          curveColor(curveColor),
          offset(offsetCounter),
          offsetEnd(offsetCounter + controlPoints.size()) {
    update();
    offsetCounter = offsetEnd;
}

void BezierCurve::update() {
    curvePoints.clear();
    plotBezier(controlPoints);
}

void BezierCurve::setPicked(int i, vec3 picked) {
    controlPoints[i - offset] = picked;
    update();
}

bool isFlat(const PointList &mesh) {
    for (int i = 1; i < mesh.size() - 1; ++i) {
        if (Line(mesh[i + 1] - mesh[i], mesh[i] - mesh[i - 1]).magnitude > EPSILON)
            return false;
    }
    return true;
}

void BezierCurve::recurse(const PointList &curve1, const PointList &curve2, PointList &resultList) const {
    PointList curve1A(curve1.begin(), curve1.begin() + curve1.size() / 2 + 1);
    PointList curve1B(curve1.begin() + curve1.size() / 2, curve1.end());

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
    intersectsRecursive(this->curvePoints, other.curvePoints, resultList);
    return resultList;
}

void BezierCurve::draw() const {
    drawCurve();
    drawMesh();
    drawPoints(GL_RENDER);
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