//
//  graphics.hpp
//  KOL_sim
//
//  Created by Omer Mustel on 4/20/22.
//

#ifndef graphics_hpp
#define graphics_hpp

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#endif
#include <cmath>



namespace KOL_sim {


// A camera.
class Camera {
    double theta;      // determines the x and z positions
    double y;          // the current y position
    double dTheta;     // increment in theta for swinging the camera around
    double dy;         // increment in y for moving the camera up/down
    friend class Simulation;
    friend class Mass;
    friend class Spring;
    
public:
    
    Camera(): theta(-0.4), y(3), dTheta(0.04), dy(0.2) {}
    void operator=(Camera &other);
    double getX() {return 10 * cos(theta);}
    double getY() {return y;}
    double getZ() {return 10 * sin(theta);}
    void moveRight() {theta += dTheta;}
    void moveLeft() {theta -= dTheta;}
    void moveUp() {y += dy;}
    void moveDown() {if (y > dy) y -= dy;}
};


// A ball.
class Ball {
    double radius;
    GLfloat* color;
    double maximumHeight;
    double x;
    double y;
    double z;
    int direction;
    
    friend class Simulation;
    friend class Mass;
    friend class Spring;
    
public:
    int ID;
    Ball(int i, double r, GLfloat* c, double x, double h, double z): ID(i),
      radius(r), color(c), maximumHeight(h), direction(-1),
      y(h), x(x), z(z) {
    }
    void update(double new_x, double new_y, double new_z);
    
};


// A checkerboard.
class Checkerboard {
    int displayListId;
    int width;
    int depth;
    friend class Simulation;
    friend class Mass;
    friend class Spring;
    
public:
    Checkerboard() : width(8), depth(8) {}
    Checkerboard(int width, int depth): width(width), depth(depth) {}
    double centerx() {return width / 2;}
    double centerz() {return depth / 2;}
    void create();
    void draw();
};


class Graphics {
    static Camera* camera;
    friend class Simulation;
    friend class Mass;
    friend class Spring;
public:
    Graphics(Camera* cam);
    // Application-specific initialization: Set up global lighting parameters
    // and create display lists.
    void init(Checkerboard &checkerboard);
    
    // On reshape, constructs a camera that perfectly fits the window.
    static void reshape(GLint w, GLint h);
    
    // Requests to draw the next frame.
    static void timer(int v);
    
    // Moves the camera according to the key pressed, then ask to refresh the
    // display.
    static void special(int key, int, int);

    static void key(unsigned char key, int x, int y);

    
};

} //namespace KOL_sim

#endif /* graphics_hpp */


