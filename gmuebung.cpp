#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cassert>
#include <glm/glm.hpp>
#include <vector>
#include <array>

#if !defined(BUFSIZE)
#define BUFSIZE 512
#endif

using namespace std;
using namespace glm;

const int num_points = 5;
const int recursions = 4;
array<vec3, num_points> points;
int picked_pos = -1;

void drawPoints(GLenum mode) {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(8.0);

    for (int i = 0; i < num_points; i++) {
        if (mode == GL_SELECT)
            glLoadName(static_cast<GLuint>(i));
        glBegin(GL_POINTS);
        glVertex3f((GLfloat) points[i].x, (GLfloat) points[i].y, (GLfloat) points[i].z);
        glEnd();
    }

    if (mode == GL_SELECT)
        glPopName();
}

void drawPolygon() {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < num_points; i++) {
        glVertex3f((GLfloat) points[i].x, (GLfloat) points[i].y, (GLfloat) points[i].z);
    }
    glEnd();
}

pair<vector<vec3>, vector<vec3>> deCasteljau(const vector<vec3> &currPoints) {
    size_t n = currPoints.size();
    vector<vector<vec3>> values(n, std::vector<vec3>(n));

    for (int x = 0; x < n; ++x) {
        for (int y = x; y < n; ++y) {
            if(x == 0) {
                values[x][y] = currPoints[y];
            } else {
                vec3 first = values[x - 1][y];
                vec3 second = values[x - 1][y - 1];
                values[x][y] = (first + second) * 0.5f;
            }
        }
    }

    vector<vec3> p1;
    vector<vec3> p2;

    for (int i = 0; i < n; ++i) {
        p1.push_back(values[i][i]);
        p2.push_back(values[n - i - 1][n - 1]);
    }

    return pair<vector<vec3>, vector<vec3>>(p1, p2);
}

void plotBezier(vector<vec3> currPoints, int k) {
    if (k == 0) {
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINE_STRIP);
        for (auto &&point : currPoints) {
            glVertex3f((GLfloat) point.x, (GLfloat) point.y, (GLfloat) point.z);
        }
        glEnd();
    } else {
        pair<vector<vec3>, vector<vec3>> divided = deCasteljau(currPoints);
        plotBezier(divided.first, k - 1);
        plotBezier(divided.second, k - 1);
    }
}

void drawCurve() {
    plotBezier(vector<vec3>(points.begin(), points.end()), recursions);
}

void drawAll(GLenum mode) {
    drawPoints(mode);
    drawPolygon();
    drawCurve();
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
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(BUFSIZE, selectBuf);
    (void) glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 8.0, 8.0, viewport);
    gluPerspective(60.0, (GLfloat) viewport[2] / (GLfloat) viewport[3], 1.0, 20.0);
    drawAll(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    hits = glRenderMode(GL_RENDER);
    int result = processHits(hits, selectBuf);

    return result;
}

void mousePress(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) // && (glutGetModifiers()==GLUT_ACTIVE_CTRL))
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

    glReadPixels(static_cast<GLint>((GLdouble) new_pos_x), static_cast<GLint>((GLdouble) new_pos_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject((GLdouble) new_pos_x, (GLdouble) new_pos_y, z, cmvm, cpm, viewport, &objx, &objy, &objz);

    if (picked_pos >= 0)
        points[picked_pos] = vec3((double) objx, (double) objy, (double) objz);

    glutPostRedisplay();
}

void display() {
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    drawAll(GL_RENDER);
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    points[0] = vec3(-1.0, 0.0, 0.0);
    points[1] = vec3(-1.0, 1.0, 0.0);
    points[2] = vec3(1.0, 1.0, 0.0);
    points[3] = vec3(1.0, 0.0, 0.0);
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
        default:break;
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
