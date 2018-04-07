#include "BezierCurve.h"
#include <utility>
#include <glm/gtc/type_ptr.hpp>

BezierCurve::BezierCurve(vector<vec3> controlPoints, int iterations)
        : controlPoints(std::move(controlPoints)),
          iterations(iterations) {
    update();
}

void BezierCurve::update() {
    curvePoints.clear();
    plotBezier(controlPoints, iterations);
}

void BezierCurve::setPicked(int i, vec3 picked) {
    controlPoints[i] = picked;
    update();
}

void BezierCurve::draw(GLenum mode) {
    drawPoints(mode);
    drawPolygon();
    drawCurve();
}

void BezierCurve::drawPoints(GLenum mode) {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(8.0);

    for (int i = 0; i < controlPoints.size(); i++) {
        if (mode == GL_SELECT)
            glLoadName(static_cast<GLuint>(i));

        glBegin(GL_POINTS);
            glVertex3fv(value_ptr(controlPoints[i]));
        glEnd();
    }

    if (mode == GL_SELECT)
        glPopName();
}

void BezierCurve::drawPolygon() {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (auto&& point : controlPoints) {
        glVertex3fv(value_ptr(point));
    }
    glEnd();
}

void BezierCurve::drawCurve() {
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (auto &&point : curvePoints) {
        glVertex3fv(value_ptr(point));
    }
    glEnd();
}

pair<vector<vec3>, vector<vec3>> BezierCurve::deCasteljau(const vector<vec3> &currPoints) {
    size_t n = currPoints.size();
    vector<vector<vec3>> values(n, std::vector<vec3>(n));

    for (int x = 0; x < n; ++x) {
        for (int y = x; y < n; ++y) {
            if(x == 0) {
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

    return pair<vector<vec3>, vector<vec3>>(p1, p2);
}

void BezierCurve::plotBezier(const vector<vec3> &currPoints, int k) {
    if (k == 0) {
        curvePoints.insert(curvePoints.end(), currPoints.begin(), currPoints.end());
    } else {
        pair<vector<vec3>, vector<vec3>> divided = deCasteljau(currPoints);
        plotBezier(divided.first, k - 1);
        plotBezier(divided.second, k - 1);
    }
}

BezierCurve::~BezierCurve() = default;
