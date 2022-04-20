////
////  sim.cpp
////  KOL_sim
////
////  Created by King on 4/9/22.
////
//
//#include "sim.hpp"
//
//
//namespace KOL_sim {
////
////
////
////Graphics Simulation::drawings = Graphics(Simulation::camera);
////int Simulation::loop = 0;
////int  Simulation::iter = 0;
////Checkerboard Simulation::checkerboard;
////Camera Simulation::camera;
////std::vector<Ball> Simulation::balls;
////std::vector<std::vector<std::vector<double>>> Simulation::massPositions;
////
////
////void Simulation::display() {
////
////    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////    glLoadIdentity();
////    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
////            checkerboard.centerx(), 0.0, checkerboard.centerz(),
////            0.0, 1.0, 0.0);
////    checkerboard.draw();
////
////
////    //read in the data
////    if (iter >= balls.size()) iter = 0;
////    for (auto& pos : massPositions) {
//////        std::cout << "loop " << loop << " pos.size() " << pos.size() << "\n";
////        if (loop == 14/0.0005/50){
////            std::cout << "KOL_sim::END_OF_SIMULATION!\n";
////            exit(0);
////        }
////        balls[iter++].update(pos[loop][0], pos[loop][2], pos[loop][1]);
////    }
////
////    loop+=1;
////    glFlush();
////    glutSwapBuffers();
////
////}
//
//
//} //namespace KOL_sim
