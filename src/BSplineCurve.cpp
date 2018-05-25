#include <glm/gtc/type_ptr.hpp>
#include <iostream>
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

    for (auto&& knot : knotVector) {
        cout << knot << endl;
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

    glColor3f(1, 0, 0);
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

//                             index     position       knots                   start d[]                 degree
PointList BSplineCurve::deBoor(int r, float t, const vector<float> &X, const PointList &currPoints, int n) const {
    PointList resultList;
    resultList.push_back(currPoints[0]);

    for (int i = 0; i < currPoints.size() - 1; ++i) {

    }

    resultList.push_back(currPoints.back());
}

void BSplineCurve::deBoorRecursive() {
    float t = largestT();

    PointList currPoints = controlPoints;

    for (int r = 0; r < degree; ++r) {
        currPoints = deBoor(r, t, knotVector, currPoints, degree);
    }
}

float BSplineCurve::largestT() {
    float max = 0;
    int maxIndex = 0;

    for (int i = 0; i < knotVector.size() - 1; ++i) {
        if(knotVector[i + 1] - knotVector[i] > max) {
            max = knotVector[i + 1] - knotVector[i];
            maxIndex = i;
        }
    }

    return (knotVector[maxIndex] + knotVector[maxIndex + 1]) / 2;
}