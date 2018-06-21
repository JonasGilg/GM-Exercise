#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "DynamicKnotVector.h"
#include "PickingOffsetManager.h"
#include "AxisAlignedBoundingBox.h"

using namespace std;

void DynamicKnotVector::setPicked(int i, const vec3 &picked) {
    if (i != offset && i != offsetEnd - 1) {
        unsigned long index = i - offset;
        if (picked.x > controlKnots[index - 1].x && picked.x < controlKnots[index + 1].x) {
            vec3 old = controlKnots[index];

            controlKnots[index] = vec3(picked.x, old.y, old.z);
            update(index);
        }
    }
}

void DynamicKnotVector::update(unsigned long index) {
    float max = bSplineCurve->knotVector.back();
    float value = controlKnots[index].x - controlKnots[0].x;
    dist = controlKnots.back().x - controlKnots.front().x;

    float result = value / dist * max;

    bSplineCurve->updateKnotVector(index, result);
}

void DynamicKnotVector::draw(GLenum mode) {
    glColor3f(1.0f, 1.0f, 1.0f);
    if(mode != GL_SELECT) {
        glBegin(GL_LINE_STRIP);
        for (auto &&point : controlKnots) {
            glVertex3fv(value_ptr(point));
        }
        glEnd();
    }

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
          offset(static_cast<const unsigned long>(PickingOffsetManager::getNewOffset(bSplineCurve->controlPoints.size() - bSplineCurve->degree + 1))),
          offsetEnd(static_cast<const unsigned long>(offset + bSplineCurve->controlPoints.size() - bSplineCurve->degree + 1)) {
    AABB aabb = AABB::createFromMesh(bSplineCurve->controlPoints);
    float yCoord = aabb.max.y + offsetUp;
    float zCoord = -15.0f;

    start = vec3(aabb.min.x, yCoord, zCoord);
    end = vec3(aabb.max.x, yCoord, zCoord);
    dist = fabs(end.x - start.x);

    float step = dist / *(bSplineCurve->knotVector.end() - 1);

    controlKnots = PointList(bSplineCurve->knotVector.size() - 2 * bSplineCurve->degree);
    for (int i = 0; i < controlKnots.size(); ++i) {
        float x = start.x + i * step;
        controlKnots[i] = vec3(x, yCoord, zCoord);
    }
}
