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
          offsetEnd(static_cast<const unsigned long>(offsetCounter + controlPoints.size())) {
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

void BSplineCurve::deBoor() {

}

void BSplineCurve::deBoorRecursive() {

}
