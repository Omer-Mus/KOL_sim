//
//  graphics.cpp
//  KOL_sim
//
//  Created by King on 4/20/22.
//

#include "graphics.hpp"
#include <iostream>
//graphics
//#ifdef __APPLE__
//#define GL_SILENCE_DEPRECATION
//#include <GLUT/glut.h>
//#endif

namespace KOL_sim {



/**********************************************************************************************************
        CAMERA CLASS
 ******************************************************************************************************/


void Camera::operator=(Camera &other) {
    this->theta = other.theta;
    y = other.y;
    dTheta = other.dTheta;
    dy = other.dy;
}


/**********************************************************************************************************
        BALL CLASS
 ******************************************************************************************************/

void Ball::update(double new_x, double new_y, double new_z) {

    x = new_x;
    y = new_y + radius;
    z = new_z;

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(x, y, z);
    glutSolidSphere(radius, 30, 30);
    glPopMatrix();
}

/**********************************************************************************************************
        BALL CLASS
 ******************************************************************************************************/

void Line::update(double x_1, double h_1, double z_1, double x_2, double h_2, double z_2) {
    std::cout << x_1;
    x1 = x_1 + 0.000000001;
    y1 = h_1;
    z2 = z_1;
    x2 = x_2;
    y2 = h_2;
    z2 = z_2;
    glLineWidth(0.02);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x1, z1, y1);
    glVertex3f(x2, z2, y2);
    glEnd();
}
/**********************************************************************************************************
        CHECKERBOARD CLASS
 ******************************************************************************************************/


void Checkerboard::create() {
    displayListId = glGenLists(1);
    glNewList(displayListId, GL_COMPILE);
    //GLfloat lightPosition[] = {4, 3, 7, 1};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glBegin(GL_QUADS);
    glNormal3d(0, 1, 0);
    for (int x = 0; x < width - 1; x++) {
        GLfloat b[] = {0, 0, 0};
        GLfloat w[] = {1, 1, 1};
        for (int z = 0; z < depth - 1; z++) {
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                         (x + z) % 2 == 0 ? b : w);
            glVertex3d(x, 0, z);
            glVertex3d(x+1, 0, z);
            glVertex3d(x+1, 0, z+1);
            glVertex3d(x, 0, z+1);
        }
    }
    glEnd();
    glEndList();
}

void Checkerboard::draw() {
    glCallList(displayListId);
}


/**********************************************************************************************************
        GRAPHICS CLASS
 ******************************************************************************************************/

Camera* Graphics::camera;

Graphics::Graphics(Camera *cam) {

    camera = cam;
    
}

void Graphics::init(Checkerboard &checkerboard) {
    GLfloat w[] = {1, 1, 1};
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, w);
    glLightfv(GL_LIGHT0, GL_SPECULAR, w);
    glMaterialfv(GL_FRONT, GL_SPECULAR, w);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    checkerboard.create();
}


void Graphics::reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
}

void Graphics::special(int key, int, int) {
    switch (key) {
        case GLUT_KEY_LEFT: camera->moveLeft(); break;
        case GLUT_KEY_RIGHT: camera->moveRight(); break;
        case GLUT_KEY_UP: camera->moveUp(); break;
        case GLUT_KEY_DOWN: camera->moveDown(); break;
    }
  glutPostRedisplay();
}


void Graphics::key(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': camera->moveLeft(); break;
        case 'd': camera->moveRight(); break;
        case 'w': camera->moveUp(); break;
        case 's': camera->moveDown(); break;
        case 'q': exit(0);
    }
    glutPostRedisplay();
}

void Graphics::timer(int v) {
    glutPostRedisplay();
    glutTimerFunc(1000/6000, timer, v);
}


} //namespace KOL_sim

