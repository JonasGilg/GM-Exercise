#ifdef WIN32

#include <windows.h>

#endif //WIN32

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cassert>
#include <glm/glm.hpp>
#include <memory> #include <vector>
#include <array>
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
}

void drawPoints() {
    curve1->drawPoints(GL_SELECT);
    curve2->drawPoints(GL_SELECT);
}

int processHits(GLint hits, GLuint buffer[]) {
    unsigned int i, j;
    GLuint names, *ptr;
    int result = -1;

    ptr = buffer;
    for (i = 0; i < hits; i++) { /*  for each hit  */
        names = *ptr;

        ptr++;
        ptr++;
        ptr++;

        for (j = 0; j < names; j++) { /*  for each name */
            printf("%d ", *ptr);

            result = (int) *ptr;

            ptr++;
        }

        printf("\n");
    }

    return result;
}

int pickPoints(int x, int y) {
    GLuint selectBuf[BUFFER_SIZE];
    GLint hits;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(BUFFER_SIZE, selectBuf);
    (void) glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 8.0, 8.0, viewport);
    gluPerspective(60.0, (GLfloat) viewport[2] / (GLfloat) viewport[3], 1.0, 20.0);
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

    glReadPixels(static_cast<GLint>((GLdouble) new_pos_x), static_cast<GLint>((GLdouble) new_pos_y), 1, 1,
                 GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject((GLdouble) new_pos_x, (GLdouble) new_pos_y, z, cmvm, cpm, viewport, &objx, &objy, &objz);

    if (picked_pos >= 0 && picked_pos < curve1->iterations) {
        curve1->setPicked(picked_pos, vec3((double) objx, (double) objy, (double) objz));
    } else if (picked_pos >= curve1->iterations) {
        curve2->setPicked(picked_pos - curve1->iterations, vec3((double) objx, (double) objy, (double) objz));
    }

    glutPostRedisplay();
}

void display() {
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    drawAll();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    curve1 = make_unique<BezierCurve>(vector<vec3>{
            vec3(-1.5, 0.0, 0.0),
            vec3(-1.5, 1.5, 0.0),
            vec3(1.5, 1.5, 0.0),
            vec3(1.5, 0.0, 0.0)
    }, 4);

    curve2 = make_unique<BezierCurve>(vector<vec3>{
            vec3(-0.5, 0.0, 0.0),
            vec3(-0.5, 0.5, 0.0),
            vec3(0.5, 0.5, 0.0),
            vec3(0.5, 0.0, 0.0)
    }, 4);
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, static_cast<GLfloat>(-5.0));
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
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 360);
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
