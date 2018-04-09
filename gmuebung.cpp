#ifdef WIN32

#include <windows.h>

#endif //WIN32

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <glm/vec3.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "BezierCurve.h"

using namespace std;
using namespace glm;

constexpr int BUFFER_SIZE = 512;

unique_ptr<BezierCurve> curve1;
unique_ptr<BezierCurve> curve2;
int picked_pos = -1;

void drawAll() {
    curve1->draw();
    curve2->draw();

    glColor3f(1.0f, 0.0f, 0.0f);
    auto intersections = curve1->intersects(*curve2);
    glBegin(GL_POINTS);
    for (auto point : intersections)
        glVertex3fv(value_ptr(point));
    glEnd();
}

void drawPoints() {
    curve1->drawPoints(GL_SELECT);
    curve2->drawPoints(GL_SELECT);
    glPopName();
}

int processHits(GLint hits, GLuint buffer[]) {
    int result = -1;

    GLuint *ptr = buffer;
    for (int i = 0; i < hits; i++) { /*  for each hit  */
        GLuint names = *ptr;
        ptr += 3;

        for (int j = 0; j < names; j++) { /*  for each name */
            result = (int) *ptr;
            ptr++;
        }
    }

    return result;
}

int pickPoints(int x, int y) {
    GLuint selectBuf[BUFFER_SIZE];
    GLint hits;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(BUFFER_SIZE, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(static_cast<double>(x), static_cast<double>(viewport[3] - y), 8.0, 8.0, viewport);
    gluPerspective(60.0, static_cast<double>(viewport[2]) / static_cast<double>(viewport[3]), 1.0, 20.0);
    drawPoints();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    hits = glRenderMode(GL_RENDER);
    int result = processHits(hits, selectBuf);

    return result;
}

void mousePress(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) // && (glutGetModifiers() == GLUT_ACTIVE_CTRL))
        picked_pos = pickPoints(x, y);

    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
        picked_pos = -1;

    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLint new_pos_x = x;
    GLint new_pos_y = viewport[3] - y;

    GLdouble cpm[16];
    glGetDoublev(GL_PROJECTION_MATRIX, cpm);

    GLdouble cmvm[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, cmvm);

    GLdouble objx, objy, objz;
    GLfloat z;

    glReadPixels(new_pos_x, new_pos_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject(new_pos_x, new_pos_y, z, cmvm, cpm, viewport, &objx, &objy, &objz);

    if (picked_pos >= 0 && picked_pos < curve2->offset) {
        curve1->setPicked(picked_pos, vec3(static_cast<float>(objx), static_cast<float>(objy), round(static_cast<float>(objz))));
    } else if (picked_pos >= curve2->offset) {
        curve2->setPicked(picked_pos, vec3(static_cast<float>(objx), static_cast<float>(objy), round(static_cast<float>(objz))));
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

    curve1 = make_unique<BezierCurve>(vector<vec3>{
            vec3(-4.0f, 0.0f, -15.0f),
            vec3(-4.0f, 4.0f, -15.0f),
            vec3(-12.0f, 4.0f, -15.0f),
            vec3(-12.0f, 0.0f, -15.0f),
            vec3(-8.0f, 0.0f, -15.0f)
    }, vec3(0.2f, 0.2f, 1.0f), vec3(1.0f, 0.0f, 1.0f), vec3(0.6f, 0.6f, 1.0f), 0);

    curve2 = make_unique<BezierCurve>(vector<vec3>{
            vec3(4.0f, 0.0f, -15.0f),
            vec3(4.0f, 4.0f, -15.0f),
            vec3(12.0f, 4.0f, -15.0f),
            vec3(12.0f, 0.0f, -15.0f),
            vec3(8.0f, 0.0f, -15.0f)
    }, vec3(0.2f, 1.0f, 0.2f), vec3(1.0f, 1.0f, 0.0f), vec3(0.6f, 1.0f, 0.6f), 5);
}

void reshape(GLsizei w, GLsizei h) {
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
        case 's':
            // do something
            glutPostRedisplay();
            break;
        case 'i':
            // do something
            glutPostRedisplay();
            break;
        case 'c':
            // do something
            glutPostRedisplay();
            break;
        case 'y':
            // do something
            glutPostRedisplay();
            break;
        case 'z':
            // do something
            glutPostRedisplay();
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
