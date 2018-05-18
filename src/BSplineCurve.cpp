#include <glm/gtc/type_ptr.hpp>
#include "BSplineCurve.h"
#include "BezierCurve.h"

unsigned long BSplineCurve::offsetCounter = 0;

using namespace std;
using namespace glm;

BSplineCurve::BSplineCurve(const PointList &controlPoints,
                           const glm::vec3 &pointColor,
                           const glm::vec3 &meshColor,
                           const glm::vec3 &curveColor)
        : controlPoints(controlPoints),
          pointColor(pointColor),
          meshColor(meshColor),
          curveColor(curveColor),
          offset(offsetCounter),
          offsetEnd(offsetCounter + controlPoints.size()) {
    degree = 3;
    int k = degree - 1;
    knotVector = vector<float>(k, 0);

    for (int i = 0; i < controlPoints.size(); ++i) {
        knotVector.push_back(i);
    }

    for (int j = 0; j < k; ++j) {
        knotVector.push_back(controlPoints.size() - 1);
    }
}

void BSplineCurve::setPicked(int i, const vec3 &picked) {
    controlPoints[i - offset] = picked;
    update();
}

void BSplineCurve::draw() const {
    for(auto&& curve : bezierCurves) {
        curve.draw();
    }

    drawMesh();
    drawPoints(GL_RENDER);
}

void BSplineCurve::drawPoints(GLenum mode) const {
    glColor3fv(value_ptr(pointColor));

    for (int i = 0; i < controlPoints.size(); i++) {
        if (mode == GL_SELECT)
            glLoadName(static_cast<GLuint>(i + offset));

        glBegin(GL_POINTS);
        glVertex3fv(value_ptr(controlPoints[i]));
        glEnd();
    }
}

void BSplineCurve::drawMesh() const {
    glColor3fv(value_ptr(meshColor));
    glBegin(GL_LINE_STRIP);
    for (auto &&point : controlPoints) {
        glVertex3fv(value_ptr(point));
    }
    glEnd();
}

void BSplineCurve::update() {
    calculateBezier();
}

void BSplineCurve::calculateBezier() {

}

//                               index       position         knots              start d[]           degree
vec3 BSplineCurve::deBoor(size_t r, float t, const vector<float> &X, const PointList &controlPoints, int n) {
    PointList d(static_cast<unsigned long>(n + 1));
    for (int i = 0; i < n + 1; ++i)
        d[i] = controlPoints[i + r - n];

    for (int j = 1; j < n + 1; ++j) {
        for (int b = n; b < j - 1; --b) {
            ulong i = b + r - n;

            float alpha = (t - X[i]) / (X[i + n - j + 1/*b + 1 + r - j*/] - X[i]);
            d[b] = (1.0f - alpha) * d[b - 1] + alpha * d[b];
        }
    }

    return d[n];
}

void BSplineCurve::deBoorRecursive() {

}
