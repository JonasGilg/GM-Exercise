#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "DynamicKnotVector.h"
#include "PickingOffsetManager.h"
#include "AxisAlignedBoundingBox.h"

void DynamicKnotVector::setPicked(int i, const vec3 &picked) {
    controlKnots[i - offset];
}

void DynamicKnotVector::update() {

}

void DynamicKnotVector::draw(GLenum mode) {
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_LINE_STRIP);
    for (auto &&point : controlKnots) {
        glVertex3fv(value_ptr(point));
    }
    glEnd();

    for (int i = 0; i < controlKnots.size(); i++) {
        if (mode == GL_SELECT) {

            glLoadName(static_cast<GLuint>(i + offset));
        }

        glBegin(GL_POINTS);
        glVertex3fv(value_ptr(controlKnots[i]));
        glEnd();
    }
}

const float offsetUp = 2.0f;

DynamicKnotVector::DynamicKnotVector(BSplineCurve *bSplineCurve)
        : bSplineCurve(bSplineCurve),
          offset(PickingOffsetManager::getNewOffset(bSplineCurve->controlPoints.size())),
          offsetEnd(offset + PickingOffsetManager::getNewOffset(bSplineCurve->controlPoints.size()))
{
    AABB aabb = AABB::createFromMesh(bSplineCurve->controlPoints);
    float yCoord = aabb.max.y + offsetUp;
    float zCoord = -15.0f;

    start = vec3(aabb.min.x, yCoord, zCoord);
    end = vec3(aabb.max.x, yCoord, zCoord);
    dist = fabs(end.x - start.x);

    float step = dist/ *(bSplineCurve->knotVector.end()-1);
    std::cout << step << std::endl;

    controlKnots = PointList(bSplineCurve->controlPoints.size());
    for (int i = 0; i < controlKnots.size(); ++i) {
        float x = start.x+i*step;
        controlKnots[i] = vec3(x,yCoord,zCoord);
        std::cout << x << std::endl;
    }

}
