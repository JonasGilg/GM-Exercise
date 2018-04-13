#pragma once

#include <string>
#include <GL/gl.h>
#include <GL/glut.h>

void drawText(std::string text, int xFromLeft, int yFromTop, int windowWidth, int windowHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(xFromLeft, yFromTop + 20);
    for (char i : text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);
    }
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}