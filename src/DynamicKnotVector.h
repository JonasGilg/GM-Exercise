#pragma once

#include "BSplineCurve.h"
#include <glm/vec3.hpp>

using namespace glm;

class DynamicKnotVector {
    BSplineCurve *bSplineCurve;
    PointList controlKnots;



    vec3 start;
    vec3 end;
    float dist;

public:
    const unsigned long offset;
    const unsigned long offsetEnd;
    DynamicKnotVector(BSplineCurve *bSplineCurve);

    void setPicked(int i, const vec3 &picked);
    void update(int index);
    void draw(GLenum mode);

    float sumOfWeights(std::vector<float> knots, int degree);
};