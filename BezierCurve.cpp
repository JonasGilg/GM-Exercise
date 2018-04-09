#include "BezierCurve.h"
#include "AxisAlignedBoundingBox.h"

#include <utility>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <sstream>
#include <typeinfo>

BezierCurve::BezierCurve(vector<vec3> controlPoints, vec3 pointColor, vec3 meshColor, vec3 curveColor, int offset)
        : controlPoints(move(controlPoints)),
          pointColor(pointColor),
          meshColor(meshColor),
          curveColor(curveColor),
          offset(offset) {
    update();
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

optional<vec3> lineIntersection(const pair<vec3, vec3> &a, const pair<vec3, vec3> &b) {
    vec3 u = a.second - a.first;
    vec3 v = b.second - b.first;
    vec3 w = a.first - b.first;
    float d = perp(u, v);

    float sI = perp(v, w) / d;
    if (sI < 0 || sI > 1)
        return {};

    float tI = perp(u, w) / d;
    if (tI < 0 || tI > 1)
        return {};

    return a.first + sI * u;
}

bool isFlat(const vector<vec3> &mesh, float epsilon) {
    for (int i = 1; i < mesh.size() - 1; ++i) {
        auto length = length2((mesh[i + 1] - mesh[i]) - (mesh[i] - mesh[i - 1]));
        if (length > epsilon)
            return false;
    }
    return true;
}

vector<vec3> BezierCurve::intersectsRecursive(const vector<vec3> &v1, const vector<vec3> &v2, float epsilon) const {
    auto v1BB = AABB::createFromMesh(v1);
    auto v2BB = AABB::createFromMesh(v2);

    if (v1BB.intersects(v2BB)) {
        if (!isFlat(v1, epsilon)) {
            auto result = deCasteljau(v1);
            auto i1 = intersectsRecursive(result.first, v2, epsilon);
            auto i2 = intersectsRecursive(result.second, v2, epsilon);
            i1.insert(i1.end(), i2.begin(), i2.end());
            return i1;
        } else if (!isFlat(v2, epsilon)) {
            auto result = deCasteljau(v2);
            auto i1 = intersectsRecursive(v1, result.first, epsilon);
            auto i2 = intersectsRecursive(v1, result.second, epsilon);
            i1.insert(i1.end(), i2.begin(), i2.end());
            return i1;
        } else {
            auto result = lineIntersection({v1.front(), v1.back()}, {v2.front(), v2.back()});
            auto resultVector = vector<vec3>();

            if (result)
                resultVector.push_back(result.value());

            return resultVector;
        }
    } else return {};
}

vector<vec3> BezierCurve::intersects(const BezierCurve &other) const {
    return intersectsRecursive(this->controlPoints, other.controlPoints, 0.001f);
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
    if (isFlat(currPoints, 0.001f)) {
        curvePoints.insert(curvePoints.end(), currPoints.begin(), currPoints.end());
    } else {
        auto result = deCasteljau(currPoints);
        plotBezier(result.first);
        plotBezier(result.second);
    }
}

string BezierCurve::toString() const {
    ostringstream result;

    for (auto&& point : controlPoints)
        result << "(" << point.x << ", " << point.y << ", " << point.z << ")" << "\n";

    result << endl;
    return result.str();
}
