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


namespace KOL_sim {



class Simulation {
    
    std::vector<double> GRAVITY = {0,0,-9.81};
    double lim;
    double T;
    double DT;
    double damping = 1;
    void time_inc() { this->T += this->DT; }
    
    double friction_s;
    double friction_k;
    double myu;
//    static std::vector<double> normalForce;
    bool fric;
//    void createFriction(double fs=0, double fk=0, double m=0, std::vector<double> normal=normalForce) {
//        double norm = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));
//        for(int i=0; i<3; ++i)
//            normalForce[i] = normalForce[i]/norm;
//    }
    
    
    // FREIND MEMBERS
    friend class Graphics;
    friend class Ball;
    friend class Camera;
    friend class Checkerboard;
    friend class Mass;
    friend class Spring;

    
    

/*-------------------------------------------------------------------*/
/*          DISPLAY GLUT                                             */
/* Must be initialzed globally for GLUT to run. Since GLUT is a C    */
/* program It cannot be initialzed as a class memeber (not even      */
/* staticly)                                                         */
/*-------------------------------------------------------------------*/
    
    
    inline static Camera camera;
    inline static Graphics drawings = Graphics(&camera);
    inline static int loop = 0;
    inline static int  iter = 0;
    inline static int  iter_lines = 0;
    inline static Checkerboard checkerboard;
    inline static std::vector<Ball> balls;
    inline static std::vector<Line> lines;
    inline static std::vector<std::vector<std::vector<double>>> massPositions;
    
    inline static void display() {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
                checkerboard.centerx(), 0.0, checkerboard.centerz(),
                0.0, 1.0, 0.0);
        checkerboard.draw();


        //read in the data
        if (iter >= balls.size()) iter = 0;
        if (iter_lines >= lines.size()) iter_lines = 0;
        for (auto& pos : massPositions) {
            if (loop == massPositions[0].size()) {
                std::cout << "KOL_sim::END_OF_SIMULATION\nKOL_sim::RESTARTING (Press q to exit)\n";
                loop=0;
                iter=0;
//                iter_lines = 0;
            }

            if (!(loop >= pos.size()))
                balls[iter++].update(pos[loop][0], pos[loop][2], pos[loop][1]);

        }
        
//        for(int i=0; i<massPositions.size(); ++i) {
//            if (loop == massPositions[0].size()) {
//                std::cout << "KOL_sim::END_OF_SIMULATION\nKOL_sim::RESTARTING (Press q to exit)\n";
//                loop=0;
//                iter=0;
//                iter_lines = 0;
//            }
//            if (!(loop >= massPositions[i].size()))
//                balls[iter++].update(massPositions[i][loop][0], massPositions[i][loop][2], massPositions[i][loop][1]);
//            if (!(i + 2 >= massPositions.size())) {
//                std::cout << massPositions[i][loop][0] << " massPositions[i][loop][0]\n";
//                lines[iter_lines++].update(massPositions[i][loop][0], massPositions[i][loop][2], massPositions[i][loop][1],
//                                           massPositions[i+2][loop][0], massPositions[i+1][loop][2], massPositions[i+1][loop][1]);
//            }
//
//        }

        loop+=1;
        glFlush();
        glutSwapBuffers();

    }
    
/*-------------------------------------------------------------------*/
/*          END DISPLAY GLUT                                         */
/*-------------------------------------------------------------------*/
    
    
    
public:
    double getT() {return lim;}
    double getDT() {return DT;}
    
    //GLUT wrapper
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
    
    //base constructor
    Simulation(double time, double time_d, std::vector<double> normal={0,0,1}, bool f=true) :
    lim(time), DT(time_d), fric(f) {}
    
    std::vector<std::vector<std::vector<double>>> simulate(std::vector<Spring> springs, std::vector<Mass> &masses)
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
        
        int i = 0, i2=0;
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

                if (s.actuateSpr) //actuate springs
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
                balls.push_back(Ball(++i, m.radius, m.color, m.position()[0], m.position()[2], m.position()[1]));
            
            // lines vertecis
//             for (int k=0; k < masses.size()-1; ++k)
//                 for (int j = k + 1; j < masses.size(); ++j)
//                    lines.push_back(Line(++i2, masses[k].position()[0], masses[k].position()[2], masses[k].position()[1], masses[j].position()[0], masses[j].position()[2], masses[j].position()[1]));

        
        }  // end Simulation Loop
        return massPositions;
        
    }


};


} //namespace KOL_sim

#endif /* sim_hpp */
