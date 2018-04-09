#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <GL/gl.h>
#include <experimental/optional>

using namespace std;
using namespace std::experimental;
using namespace glm;

class BezierCurve {
public:
    BezierCurve(vector<vec3> controlPoints, int iterations, int offset);

    void update();
    void draw() const;
    void drawPoints(GLenum mode) const;

    void setPicked(int i, vec3 picked);

    vector<vec3> intersects(const BezierCurve& other) const;

    virtual ~BezierCurve() = default;

    const int iterations;
    const int offset;

private:
    void plotBezier(const vector<vec3> &currPoints, int k);
    pair<vector<vec3>, vector<vec3>> deCasteljau(const vector<vec3> &currPoints) const;
    vector<vec3> intersectsRecursive(const vector<vec3> &v1, const vector<vec3> &v2, float epsilon) const;

    void drawPolygon() const;
    void drawCurve() const;

    vector<vec3> controlPoints;
    vector<vec3> curvePoints;

    static constexpr vec3 POINT_COLOR = vec3(1, 0, 0);
    static constexpr vec3 POLYGON_COLOR = vec3(0, 1, 0);
    static constexpr vec3 CURVE_COLOR = vec3(0, 0, 1);
};