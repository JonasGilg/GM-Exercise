#pragma once

#include "BSplineCurve.h"
#include <glm/vec3.hpp>

using namespace glm;

class DynamicKnotVector {
    BSplineCurve *bSplineCurve;
    PointList controlKnots;

    const unsigned long offset;
    const unsigned long offsetEnd;

    vec3 start;
    vec3 end;
    float dist;

public:
    DynamicKnotVector(BSplineCurve *bSplineCurve);

    void setPicked(int i, const vec3 &picked);
    void update();
    void draw(GLenum mode);

    float sumOfWeights(std::vector<float> knots, int degree);
};