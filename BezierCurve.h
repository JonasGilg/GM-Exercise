#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <GL/gl.h>

using namespace std;
using namespace glm;

class BezierCurve {
public:
    BezierCurve(vector<vec3> controlPoints, int iterations, int offset);

    void update();
    void draw() const;
    void drawPoints(GLenum mode) const;

    void setPicked(int i, vec3 picked);

    virtual ~BezierCurve() = default;

    const int iterations;
    const int offset;
private:
    void plotBezier(const vector<vec3> &currPoints, int k);
    pair<vector<vec3>, vector<vec3>> deCasteljau(const vector<vec3> &currPoints);

    void drawPolygon() const;
    void drawCurve() const;

    vector<vec3> controlPoints;
    vector<vec3> curvePoints;

    static const vec3 POINT_COLOR;
    static const vec3 POLYGON_COLOR;
    static const vec3 CURVE_COLOR;
};