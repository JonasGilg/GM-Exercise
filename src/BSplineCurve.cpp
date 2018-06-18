#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "BSplineCurve.h"
#include "BezierCurve.h"
#include "PickingOffsetManager.h"

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
          offset(PickingOffsetManager::getNewOffset(controlPoints.size())),
          offsetEnd(offset + controlPoints.size()) {
    degree = 3;
    size_t max = degree + controlPoints.size() + 1;
    int number = 0;
    for (int i = 0; i < max; ++i) {
        if(i < degree) {
            knotVector.push_back(number);
        } else if(i < max - degree - 1) {
            knotVector.push_back(number);
            number++;
        } else {
            knotVector.push_back(number);
        }
    }

    update();
}


void BSplineCurve::setPicked(int i, const vec3 &picked) {
    controlPoints[i - offset] = picked;
    update();
}

void BSplineCurve::draw() const {
    drawMesh();
    drawPoints(GL_RENDER);

    for(auto&& curve : bezierCurves) {
        curve.draw();
    }

    glColor3f(1, 0, 0);
}

void BSplineCurve::drawPoints(GLenum mode) const {
    glColor3fv(value_ptr(pointColor));

    for (int i = 0; i < controlPoints.size(); i++) {
        if (mode == GL_SELECT) {

            glLoadName(static_cast<GLuint>(i + offset));
        }

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
    for(auto&& knot : knotVector)
        cout << knot << ", ";
    cout<<endl;
    calculateBezier();
}

void BSplineCurve::calculateBezier() {
    PointList resultList = controlPoints;
    vector<float> x = knotVector;

    int r = largestTIndex(x);

    while (r != -1) {
        float t = (x[r] + x[r - 1]) / 2;
        resultList = deBoor(r, t, x, resultList);

        float xr = x[r];
        x.insert(x.begin() + r, xr);
        r = largestTIndex(x);
    }

    bezierCurves.clear();
    size_t numBezier = (resultList.size() - 1) / degree;
    for (int i = 0; i < numBezier; ++i) {
        bezierCurves.emplace_back(PointList(resultList.begin() + i * degree, resultList.begin() + (i * degree) + degree + 1), vec3(1, 1, 1), vec3(1, 1, 1), vec3(0, 1, 0));
    }
}

int BSplineCurve::largestTIndex(const vector<float>& x) {
    float currNum = x[0];
    int counter = 1;

    for (int i = 1; i < x.size(); ++i) {
        if(x[i] == currNum) {
            counter++;
        } else {
            if(counter < degree) {
                return i - 1;
            } else {
                counter = 1;
                currNum = x[i];
            }
        }
    }

    return -1;
}

float BSplineCurve::alpha(int i, int j, float t, const vector<float> &x)const {
    return (t - x[i]) / (x[i + degree - j + 1] - x[i]);
}

vec3 BSplineCurve::d(const vec3& d0, const vec3& d1,float alpha)const {
    return (1 - alpha) * d0 + alpha * d1;
}

//                             index     position       knots                   start d[]                 degree
PointList BSplineCurve::deBoor(int r, float t, const vector<float> &X, const PointList &currPoints) const {
    PointList resultPoints = PointList(currPoints.size() + 1);

    int startIndex = r - degree;

    for (int i = 0; i < resultPoints.size(); ++i) {
        if(i <= startIndex) {
            resultPoints[i] = currPoints[i];
        } else if(i >= r) {
            resultPoints[i] = currPoints[i - 1];
        } else {
            resultPoints[i] = d(currPoints[i - 1], currPoints[i], alpha(i, 0, t, X));
        }
    }

    return resultPoints;
}

void BSplineCurve::updateKnotVector(int index, float input) {

    knotVector[index+degree] = input;

    update();
}


