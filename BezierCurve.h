#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <GL/gl.h>

using namespace std;
using namespace glm;

class BezierCurve {
public:
    BezierCurve(vector<vec3> controlPoints, vec3 pointColor, vec3 meshColor, vec3 curveColor, int offset);

    void update();
    void draw() const;
    void drawPoints(GLenum mode) const;

    void setPicked(int i, vec3 picked);

    vector<vec3> intersects(const BezierCurve& other) const;

    virtual ~BezierCurve() = default;

    const int offset;

private:
    void plotBezier(const vector<vec3> &currPoints);
    pair<vector<vec3>, vector<vec3>> deCasteljau(const vector<vec3> &currPoints) const;
    vector<vec3> intersectsRecursive(const vector<vec3> &v1, const vector<vec3> &v2, float epsilon) const;

    void drawMesh() const;
    void drawCurve() const;

    vector<vec3> controlPoints;
    vector<vec3> curvePoints;

    const vec3 pointColor;
    const vec3 meshColor;
    const vec3 curveColor;
};