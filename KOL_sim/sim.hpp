//
//  sim.hpp
//  KOL_sim
//
//  Created by Omer Mustel on 4/9/22.
//

#ifndef sim_hpp
#define sim_hpp

#include <iostream>
#include "mass.hpp"
#include "spring.hpp"
#include <algorithm>
#include <functional>
#include <fstream>
#include <iterator>
//#include "sim_test.cpp"

//#ifdef __APPLE__
//#define GL_SILENCE_DEPRECATION
//#include <GLUT/glut.h>
//#endif

namespace KOL_sim {



Camera camera;
Graphics drawings = Graphics(&camera);
int loop = 0;
int  iter = 0;
Checkerboard checkerboard;

std::vector<Ball> balls;
std::vector<std::vector<std::vector<double>>> massPositions;


void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
            checkerboard.centerx(), 0.0, checkerboard.centerz(),
            0.0, 1.0, 0.0);
    checkerboard.draw();


    //read in the data
    if (iter >= balls.size()) iter = 0;
    for (auto& pos : massPositions) {
//        std::cout << "loop " << loop << " pos.size() " << pos.size() << "\n";
        if (loop == 7/0.0005/50){
            std::cout << "KOL_sim::END_OF_SIMULATION!\n";
            exit(0);
        }
        if (!(loop >= pos.size()))
            balls[iter++].update(pos[loop][0], pos[loop][2], pos[loop][1]);
    }

    loop+=1;
    glFlush();
    glutSwapBuffers();

}




class Simulation {
    std::vector<double> GRAVITY = {0,0,-9.81};
    double lim = 0;
    double T = 0;
    double DT;
    double damping = 1;
    void time_inc() { this->T += this->DT; }
    double friction_s;
    double friction_k;
    double myu;
//    static std::vector<double> normalForce;
    bool fric;
    bool actuateSpringe;
//    void createFriction(double fs=0, double fk=0, double m=0, std::vector<double> normal=normalForce) {
//        double norm = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));
//        for(int i=0; i<3; ++i)
//            normalForce[i] = normalForce[i]/norm;
//    }
    friend class Graphics;
    friend class Ball;
    friend class Camera;
    friend class Checkerboard;
    friend class Mass;
    friend class Spring;
    
public:
    void start(int &argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowPosition(80, 80);
        glutInitWindowSize(800, 600);
        glutCreateWindow("Bouncing Balls");
        glutDisplayFunc(display);
        glutReshapeFunc(drawings.reshape);
        glutSpecialFunc(drawings.special);
        glutTimerFunc(100, drawings.timer, 0);
        glutKeyboardFunc(drawings.key);
        drawings.init(checkerboard);
        glutMainLoop();
        
    }
    Simulation(double time, double time_d, std::vector<double> normal={0,0,1}, bool f=true, bool actuate = false)
    {
        this->lim = time;
        this->DT = time_d;
        fric = f;
        actuateSpringe = actuate;
//        normalForce = normal;
    }
    void simulate(std::vector<Spring> springs, std::vector<Mass> &masses, int& argc, char **argv)
    {

        int c=0;
        //FRICTION HERE
//        createFriction();

        //Create a vector that will hold the new positiosn of all the masses.
        for (auto& m : masses) {
            std::vector<std::vector<double>> posVec;
            std::vector<double> initPos = { m.p[0], m.p[1], m.p[2] };
            posVec.push_back(initPos);
            massPositions.push_back(posVec);
        }
        
        int i = 0;
        for (auto& m : masses)
            balls.push_back(KOL_sim::Ball(++i, m.radius, m.color, m.position()[0], m.position()[2], m.position()[1]));
        
        

        ///<Start Simulation Loop>
        while (T < lim) {
            /**************************************************
              Part 4.a.
            **************************************************/
            //compute the F forces
            for (auto& m : masses) {

                std::vector<double> temp1;

                for (int i=0; i<3; ++i)
                    temp1.push_back(m.m * GRAVITY[i]);
                for (int i=0; i<3; ++i)
                    m.F[i] += temp1[i];



                /* TODO: FRICITON

                if (m.p.back() == 0) {
                    Apply Friction....
                }

                 */
                //apply ground force
                if (m.p.back() < 0) {
                    std::vector<double> c_pos = {0,0,100'000*m.p[2]*(-1)};
                    for (int i=0; i<3; i++)
                        m.F[i] += c_pos[i];
                }
            }

            //iterate over the springs
            for (auto& s : springs) {

                if (actuateSpringe) //actuate springs
                    s.actuateSpring(T);

                double f = s.springForce();
                std::vector<double> temp;
                double magnitude = 0;
                for (int i=0; i<3; i++) {
                    temp.push_back(s.m2->p[i]-s.m1->p[i]);
                    magnitude += pow(s.m1->p[i]-s.m2->p[i], 2);
                }
                std::vector<double> tempUnit(temp);
                for (int i=0; i<3; i++) {
                    if (magnitude <= 0) std::cout << magnitude << "\n";
                    else tempUnit[i] /= sqrt(magnitude); // move outside
                }
                for (int i=0; i<3; i++) {
                    tempUnit[i] *= f;
                    s.m1->F[i] -= tempUnit[i];
                    s.m2->F[i] += tempUnit[i];
                }
            }
            /**************************************************
              Part 4.b.
            **************************************************/
            int mass_append = 1;
            int massID = 0;

            for (auto& m : masses) {

                m.update_acceleration();
                m.update_velocity(DT, damping);
                m.update_position(DT);

                if (c == 50) {
                    std::vector<double> newPos = { m.p[0]+3, m.p[1]+3, m.p[2]};
                    massPositions[massID].push_back(newPos);
                    massID+=1;
                }

                mass_append++;
                m.update_damping();
            }
            T += DT;

            if (c==50) c=0;
            c++;

            for (auto& m : masses)
                m.F = (std::vector<double>) {0, 0, 0};
            for (auto& m : masses) {
                std::vector<std::vector<double>> posVec;
                std::vector<double> initPos = { m.p[0], m.p[1], m.p[2] };
                posVec.push_back(initPos);
                massPositions.push_back(posVec);
            }  
            for (auto& m : masses)
                balls.push_back(KOL_sim::Ball(++i, m.radius, m.color, m.position()[0], m.position()[2], m.position()[1]));
        
        }  // end Simulation Loop
        
    }


};


} //namespace KOL_sim

#endif /* sim_hpp */



//#ifndef sim_hpp
//#define sim_hpp
//
//#include <iostream>
//#include "mass.hpp"
//#include "spring.hpp"
//#include <algorithm>
//#include <functional>
//#include <fstream>
//#include <iterator>
//#include <stdlib.h>
//
//#ifdef __APPLE__
//#define GL_SILENCE_DEPRECATION
//#include <GLUT/glut.h>
//#endif
//
//namespace KOL_sim {
//
//GLfloat WHITE[] = {1, 1, 1};
//GLfloat RED[] = {1, 0, 0};
//GLfloat GREEN[] = {0, 1, 0};
//GLfloat MAGENTA[] = {1, 0, 1};
//GLfloat BLACK[] = {0, 0, 0};
//
//
//// A checkerboard.
//class Checkerboard {
//  int displayListId;
//  int width;
//  int depth;
//public:
//  Checkerboard(int width, int depth): width(width), depth(depth) {}
//    Checkerboard(): width(8), depth(8) {}
//  double centerx() {return width / 2;}
//  double centerz() {return depth / 2;}
//  void create() {
//    displayListId = glGenLists(1);
//    glNewList(displayListId, GL_COMPILE);
//    glBegin(GL_QUADS);
//    glNormal3d(0, 1, 0);
//    for (int x = 0; x < width - 1; x++) {
//      for (int z = 0; z < depth - 1; z++) {
//        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
//                     (x + z) % 2 == 0 ? BLACK : WHITE);
//        glVertex3d(x, 0, z);
//        glVertex3d(x+1, 0, z);
//        glVertex3d(x+1, 0, z+1);
//        glVertex3d(x, 0, z+1);
//      }
//    }
//    glEnd();
//    glEndList();
//  }
//  void draw() {
//    glCallList(displayListId);
//  }
//};
//
//
//// A ball.
//class Ball {
//  double radius;
//  GLfloat* color;
//  double maximumHeight;
//  double x;
//  double y;
//  double z;
//  int direction;
//public:
//    int ID;
//    Ball(int i, double r, GLfloat* c, double x, double h, double z): ID(i),
//      radius(r), color(c), maximumHeight(h), direction(-1),
//      y(h), x(x), z(z) {
//    }
//    void update(double new_x, double new_y, double new_z) {
//    //    y += direction * 0.05;
//    //    if (y > maximumHeight) {
//    //      y = maximumHeight; direction = -1;
//    //    } else if (y < radius) {
//    //      y = radius; direction = 1;
//    //    }
//
//      x = new_x;
//      y = new_y + radius;
//      z = new_z;
//
//    glPushMatrix();
//    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
//    glTranslated(x, y, z);
//    glutSolidSphere(radius, 30, 30);
//    glPopMatrix();
//    }
//};
//
//
//// A camera.
//class Camera {
//  double theta;      // determines the x and z positions
//  double y;          // the current y position
//  double dTheta;     // increment in theta for swinging the camera around
//  double dy;         // increment in y for moving the camera up/down
//public:
//  Camera(): theta(0), y(3), dTheta(0.04), dy(0.2) {}
//  double getX() {return 10 * cos(theta);}
//  double getY() {return y;}
//  double getZ() {return 10 * sin(theta);}
//  void moveRight() {theta += dTheta;}
//  void moveLeft() {theta -= dTheta;}
//  void moveUp() {y += dy;}
//  void moveDown() {if (y > dy) y -= dy;}
//};
//
//
//
//static Checkerboard checkerboard;
//static Camera camera;
//static std::vector<Ball> balls;
//
//static void init() {
//  glEnable(GL_DEPTH_TEST);
//  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
//  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
//  glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
//  glMaterialf(GL_FRONT, GL_SHININESS, 30);
//  glEnable(GL_LIGHTING);
//  glEnable(GL_LIGHT0);
//  checkerboard.create();
//}
//
//
//static std::vector<std::vector<std::vector<double>>> massPositions;
//static int loop;
//static int iter;
//static void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
//              checkerboard.centerx(), 0.0, checkerboard.centerz(),
//            0.0, 1.0, 0.0);
//    checkerboard.draw();
//
//
//    //read in the data
//    if (iter >= balls.size()) iter = 0;
//    for (auto& pos : massPositions) {
//        std::cout << "loop " << loop << " pos.size() " << pos.size() << "\n";
//        if (loop == 14/0.0005/50){
//            std::cout << "KOL_sim::END_OF_SIMULATION!\n";
//            exit(0);
//        }
//        balls[iter++].update(pos[loop][0], pos[loop][2], pos[loop][1]);
//    }
//
//    loop+=1;
//    glFlush();
//    glutSwapBuffers();
//
//}
//
//
//static void reshape(GLint w, GLint h) {
//  glViewport(0, 0, w, h);
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
//  glMatrixMode(GL_MODELVIEW);
//}
//
//// Requests to draw the next frame.
//static void timer(int v) {
//  glutPostRedisplay();
//  glutTimerFunc(1000/6000, timer, v);
//}
//
//
//    // Moves the camera according to the key pressed, then ask to refresh the
//    // display.
//static void special(int key, int, int) {
//  switch (key) {
//    case GLUT_KEY_LEFT: camera.moveLeft(); break;
//    case GLUT_KEY_RIGHT: camera.moveRight(); break;
//    case GLUT_KEY_UP: camera.moveUp(); break;
//    case GLUT_KEY_DOWN: camera.moveDown(); break;
//  }
//  glutPostRedisplay();
//}
//
//
//static void key(unsigned char key, int x, int y) {
//  switch (key) {
//    case 'a': camera.moveLeft(); break;
//    case 'd': camera.moveRight(); break;
//    case 'w': camera.moveUp(); break;
//    case 's': camera.moveDown(); break;
//    case 'q': exit(0);
//
//  }
//glutPostRedisplay();
//}
//
//
//
//static void begin(int &argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowPosition(80, 80);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("Bouncing Balls");
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutSpecialFunc(special);
//    glutTimerFunc(100, timer, 0);
//    glutKeyboardFunc(key);
//    init();
//    glutMainLoop();
//
//}
//
//
//class Simulation {
//    std::vector<double> GRAVITY = {0,0,-9.81};
//    double lim;
//    double T = 0;
//    double DT;
//    double damping = 1;
//    void time_inc() { this->T += this->DT; }
//    double friction_s;
//    double friction_k;
//    double myu;
////    static std::vector<double> normalForce;
//    bool fric;
//    bool actuateSpringe;
////    void createFriction(double fs=0, double fk=0, double m=0, std::vector<double> normal=normalForce) {
////        double norm = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));
////        for(int i=0; i<3; ++i)
////            normalForce[i] = normalForce[i]/norm;
////    }
//
//
//
//
//public:
//
//    Simulation(double time, const double time_d, std::vector<double> normal={0,0,1}, bool f=true, bool actuate = false) {
//        loop=0;
//        this->lim = time;
//        this->DT = time_d;
//        fric = f;
//        actuateSpringe = actuate;
//
//    }
//
//
//     void simulate(std::vector<Spring> springs, std::vector<Mass> &masses, int argc, char** argv) {
//
//        int c=0;
//        //FRICTION HERE
////        createFriction();
//
//        //Create a vector that will hold the new positiosn of all the masses.
//
//        int i = 0;
//        for (auto& m : masses) {
//
//            balls.push_back(Ball(++i, m.radius, m.color, m.position()[0], m.position()[2], m.position()[1]));
//        }
//
//         begin(argc, argv);
//
//        for (auto& m : masses) {
//            std::vector<std::vector<double>> posVec;
//            std::vector<double> initPos = { m.p[0], m.p[1], m.p[2] };
//            posVec.push_back(initPos);
//            massPositions.push_back(posVec);
//        }
//
//        ///<Start Simulation Loop>
//        while (T < lim) {
//            /**************************************************
//              Part 4.a.
//            **************************************************/
//            //compute the F forces
//            for (auto& m : masses) {
//
//                std::vector<double> temp1;
//
//                for (int i=0; i<3; i++)
//                    temp1.push_back(m.m * GRAVITY[i]);
//                for (int i=0; i<3; i++)
//                    m.F[i] += temp1[i];
//
//
//
//                /* TODO: FRICITON
//
//                if (m.p.back() == 0) {
//                    Apply Friction....
//                }
//
//                 */
//                //apply ground force
//                if (m.p.back() < 0) {
//                    std::vector<double> c_pos = {0,0,100'000*m.p[2]*(-1)};
//                    for (int i=0; i<3; i++)
//                        m.F[i] += c_pos[i];
//                }
//            }
//
//            //iterate over the springs
//            for (auto& s : springs) {
//
//                if (actuateSpringe) //actuate springs
//                    s.actuateSpring(T);
//
//                double f = s.springForce();
//                std::vector<double> temp;
//                double magnitude = 0;
//                for (int i=0; i<3; i++) {
//                    temp.push_back(s.m2->p[i]-s.m1->p[i]);
//                    magnitude += pow(s.m1->p[i]-s.m2->p[i], 2);
//                }
//                std::vector<double> tempUnit(temp);
//                for (int i=0; i<3; i++) {
//                    if (magnitude <= 0) std::cout << magnitude << "\n";
//                    else tempUnit[i] /= sqrt(magnitude); // move outside
//                }
//                for (int i=0; i<3; i++) {
//                    tempUnit[i] *= f;
//                    s.m1->F[i] -= tempUnit[i];
//                    s.m2->F[i] += tempUnit[i];
//                }
//            }
//            /**************************************************
//              Part 4.b.
//            **************************************************/
//            int mass_append = 1;
//            int massID = 0;
//
//            for (auto& m : masses) {
//
//                m.update_acceleration();
//                m.update_velocity(DT, damping);
//                m.update_position(DT);
//
//                if (c == 50) {
//                    std::vector<double> newPos = { m.p[0]+3, m.p[1]+3, m.p[2]};
//                    massPositions[massID].push_back(newPos);
//                    massID+=1;
//                }
//
//                mass_append++;
//                m.update_damping();
//            }
//            T += DT;
//
//            if (c==50) c=0;
//            c++;
//
//            for (auto& m : masses)
//                m.F = (std::vector<double>) {0, 0, 0};
//
//            for (auto& m : masses)
//                balls.push_back(Ball(++i, m.radius, m.color, m.position()[0], m.position()[2], m.position()[1]));
//
//
//        }
//
//
//        // end Simulation Loop
//
//
//    }
//
//
//};
//
//
//} //namespace KOL_sim
//
//#endif /* sim_hpp */
