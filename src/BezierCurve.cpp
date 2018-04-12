#include <glm/gtc/type_ptr.hpp>
#include <experimental/optional>
#include <iostream>

#include "BezierCurve.h"
#include "AxisAlignedBoundingBox.h"
#include "Line.h"

using namespace std;
using namespace experimental;
using namespace glm;

constexpr float EPSILON = 0.001f;

unsigned long BezierCurve::offsetCounter = 0;

BezierCurve::BezierCurve(const vector<vec3> &controlPoints,
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

float perp(vec3 u, vec3 v) {
    return u.x * v.y - u.y * v.x;
}

optional<vec3> lineIntersection(const Line &a, const Line &b) {
    vec3 u = a.direction;
    vec3 v = b.direction;
    vec3 w = a.start - b.start;
    float d = perp(u, v);

    float sI = perp(v, w) / d;
    if (sI < 0 || sI > 1)
        return {};

    float tI = perp(u, w) / d;
    if (tI < 0 || tI > 1)
        return {};

    return a.start + sI * u;
}

bool isFlat(const vector<vec3> &mesh) {
    for (int i = 1; i < mesh.size() - 1; ++i) {
        if (Line(mesh[i + 1] - mesh[i], mesh[i] - mesh[i - 1]).magnitude > EPSILON)
            return false;
    }
    return true;
}

vector<vec3> BezierCurve::recurse(const vector<vec3> &curve1, const vector<vec3> &curve2) const {
    auto result = deCasteljau(curve1);
    auto intersections1 = intersectsRecursive(result.first, curve2);
    auto intersections2 = intersectsRecursive(result.second, curve2);
    intersections1.insert(intersections1.end(), intersections2.begin(), intersections2.end());
    return intersections1;
}

vector<vec3> BezierCurve::intersectsRecursive(const vector<vec3> &curve1, const vector<vec3> &curve2) const {
    auto curve1BB = AABB::createFromMesh(curve1);
    auto curve2BB = AABB::createFromMesh(curve2);

    if (curve1BB.intersects(curve2BB)) {
        if (!isFlat(curve1)) {
            return recurse(curve1, curve2);
        } else if (!isFlat(curve2)) {
            return recurse(curve2, curve1);
        } else {
            auto result = lineIntersection({curve1.front(), curve1.back()}, {curve2.front(), curve2.back()});
            vector<vec3> resultVector;

            if (result)
                resultVector.push_back(*result);

            return resultVector;
        }
    } else return {};
}

vector<vec3> BezierCurve::intersects(const BezierCurve &other) const {
    return intersectsRecursive(this->controlPoints, other.controlPoints);
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

pair<vector<vec3>, vector<vec3>> BezierCurve::deCasteljau(const vector<vec3> &currPoints) const {
    size_t n = currPoints.size();
    vector<vector<vec3>> values(n, std::vector<vec3>(n));

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

    vector<vec3> p1;
    vector<vec3> p2;

    for (int i = 0; i < n; ++i) {
        p1.push_back(values[i][i]);
        p2.push_back(values[n - i - 1][n - 1]);
    }

    return {p1, p2};
}

void BezierCurve::plotBezier(const vector<vec3> &currPoints) {
    if (isFlat(currPoints)) {
        curvePoints.insert(curvePoints.end(), currPoints.begin(), currPoints.end());
    } else {
        auto c = deCasteljau(currPoints);
        plotBezier(c.first);
        plotBezier(c.second);
    }
}