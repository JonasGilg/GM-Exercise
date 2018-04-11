#pragma once

#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

#include <vector>
#include <tuple>
#include <GL/gl.h>
#include <glm/vec3.hpp>

using namespace std;
using namespace glm;

class BezierCurve {
public:
    BezierCurve(const vector<vec3> &controlPoints, vec3 pointColor, vec3 meshColor, vec3 curveColor);

    void update();
    void draw() const;
    void drawPoints(GLenum mode) const;

    void setPicked(int i, vec3 picked);

    vector<vec3> intersects(const BezierCurve& other) const;

    virtual ~BezierCurve() = default;

    unsigned long offset;
    unsigned long offsetEnd;
private:
    void plotBezier(const vector<vec3> &currPoints);
    pair<vector<vec3>, vector<vec3>> deCasteljau(const vector<vec3> &currPoints) const;
    vector<vec3> intersectsRecursive(const vector<vec3> &v1, const vector<vec3> &v2) const;

    void drawMesh() const;
    void drawCurve() const;

    vector<vec3> controlPoints;
    vector<vec3> curvePoints;

    const vec3 pointColor;
    const vec3 meshColor;
    const vec3 curveColor;

    static unsigned long offsetCounter;
};