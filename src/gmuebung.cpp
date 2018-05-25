#ifdef _WIN32

#include <windows.h>

#endif //_WIN32

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <glm/vec3.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "BezierCurve.h"
#include "Util.h"
#include "DrawUtil.h"
#include "BSplineCurve.h"

using namespace std;
using namespace glm;

constexpr int ESCAPE_KEY = 27;
constexpr int BUFFER_SIZE = 512;

int windowWidth;
int windowHeight;

vector<BezierCurve> bezierCurves;
vector<BSplineCurve> bSplines;
vector<vec3> intersections;

constexpr bool pickBezier = false;

int picked_pos = -1;

void drawAll() {
    for (auto &&curve : bezierCurves)
        curve.draw();

    for (auto &&curve : bSplines)
        curve.draw();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    for (auto &&point : intersections)
        glVertex3fv(value_ptr(point));
    glEnd();

    drawText("Made by Jonas Gilg", 10, 10, windowWidth, windowHeight);
}

void drawPoints() {
    for (auto &&curve : bezierCurves)
        curve.drawPoints(GL_SELECT);


    for (auto &&curve : bSplines)
        curve.drawPoints(GL_SELECT);

    glPopName();
}

int processHits(GLint hits, const array<GLuint, BUFFER_SIZE> &buffer) {
    int result = -1;

    const GLuint *ptr = buffer.data();
    for (int i = 0; i < hits; i++) { //for each hit
        GLuint names = *ptr;
        ptr += 3;

        for (int j = 0; j < names; j++) { //for each name
            result = (int) *ptr;
            ptr++;
        }
    }

    return result;
}

int pickPoints(int x, int y) {
    array<GLuint, BUFFER_SIZE> selectBuf{};
    GLint hits;
    array<GLint, 4> viewport{};

    glGetIntegerv(GL_VIEWPORT, viewport.data());
    glSelectBuffer(BUFFER_SIZE, selectBuf.data());
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(static_cast<double>(x), static_cast<double>(viewport[3] - y), 8.0, 8.0, viewport.data());
    gluPerspective(60.0, static_cast<double>(viewport[2]) / static_cast<double>(viewport[3]), 1.0, 20.0);
    drawPoints();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    hits = glRenderMode(GL_RENDER);
    return processHits(hits, selectBuf);
}

void mousePress(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
        picked_pos = pickPoints(x, y);

    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
        picked_pos = -1;

    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    array<GLint, 4> viewport{};
    glGetIntegerv(GL_VIEWPORT, viewport.data());

    GLint new_pos_x = x;
    GLint new_pos_y = viewport[3] - y;

    array<GLdouble, 16> cpm{};
    glGetDoublev(GL_PROJECTION_MATRIX, cpm.data());

    array<GLdouble, 16> cmvm{};
    glGetDoublev(GL_MODELVIEW_MATRIX, cmvm.data());

    GLdouble objx, objy, objz;
    GLfloat z;

    glReadPixels(new_pos_x, new_pos_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject(new_pos_x, new_pos_y, z, cmvm.data(), cpm.data(), viewport.data(), &objx, &objy, &objz);

    if (picked_pos >= 0) {
        if(pickBezier) {
            for (auto &&curve : bezierCurves) {
                if (picked_pos >= curve.offset && picked_pos < curve.offsetEnd) {
                    curve.setPicked(picked_pos, {objx, objy, round(objz)});
                }
            }
        } else {
            for (auto &&curve : bSplines) {
                if (picked_pos >= curve.offset && picked_pos < curve.offsetEnd) {
                    curve.setPicked(picked_pos, {objx, objy, round(objz)});
                }
            }
        }

        intersections.clear();
        for (int i = 0; i < bezierCurves.size(); ++i) {
            for (int j = i + 1; j < bezierCurves.size(); ++j) {
                auto intersects = bezierCurves[i].intersects(bezierCurves[j]);
                intersections.insert(intersections.end(), intersects.begin(), intersects.end());
            }
        }
    }

    glutPostRedisplay();
}

void display() {
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    drawAll();

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glShadeModel(GL_FLAT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0);

    /*bezierCurves.emplace_back(vector<vec3>{
            {-4.0f,  2.0f, -15.0f},
            {-4.0f,  6.0f, -15.0f},
            {-12.0f, 6.0f, -15.0f},
            {-12.0f, 2.0f, -15.0f},
            {-8.0f,  2.0f, -15.0f}
    }, vec3(0.2f, 0.2f, 1.0f), vec3(1.0f, 0.0f, 1.0f), vec3(0.6f, 0.6f, 1.0f));

    bezierCurves.emplace_back(vector<vec3>{
            {4.0f,  2.0f, -15.0f},
            {4.0f,  6.0f, -15.0f},
            {12.0f, 6.0f, -15.0f},
            {12.0f, 2.0f, -15.0f},
            {8.0f,  2.0f, -15.0f}
    }, vec3(0.2f, 1.0f, 0.2f), vec3(1.0f, 1.0f, 0.0f), vec3(0.6f, 1.0f, 0.6f));

    bezierCurves.emplace_back(vector<vec3>{
            {-4.0f, -6.0f, -15.0f},
            {-4.0f, -2.0f, -15.0f},
            {4.0f,  -2.0f, -15.0f},
            {4.0f,  -6.0f, -15.0f},
            {0.0f,  -6.0f, -15.0f}
    }, vec3(0.2f, 0.2f, 0.2f), vec3(1.0f, 1.0f, 1.0f), vec3(0.6f, 0.6f, 0.6f));*/

    bSplines.emplace_back(vector<vec3>{
            {-4.0f,  2.0f, -15.0f},
            {-4.0f,  6.0f, -15.0f},
            {-12.0f, 6.0f, -15.0f},
            {-12.0f, 2.0f, -15.0f},
            {-8.0f,  2.0f, -15.0f}
    }, vec3(0.2f, 0.2f, 1.0f), vec3(1.0f, 0.0f, 1.0f), vec3(0.6f, 0.6f, 1.0f));
}

void reshape(GLsizei w, GLsizei h) {
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, static_cast<double>(w) / static_cast<double>(h), 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ESCAPE_KEY:
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    constexpr int WINDOW_WIDTH = 1200;
    constexpr int WINDOW_HEIGHT = 720;
    const int windowX = (glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2;
    const int windowY = (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2;

    glutInitWindowPosition(windowX, windowY);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("GM Uebung SoSe 2018");

    init();

    glutMouseFunc(mousePress);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
