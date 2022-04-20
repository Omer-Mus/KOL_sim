//#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include "sim.hpp"
//#include "graphics.hpp"


//std::vector<double> GRAVITY = {0,0,-9.81};
static const double DT = 0.0005;
double T = 7;
GLfloat WHITE[] = {1, 1, 1};
GLfloat RED[] = {1, 0, 0};
GLfloat GREEN[] = {0, 1, 0};
GLfloat MAGENTA[] = {1, 0, 1};
GLfloat BLACK[] = {0, 0, 0};





// GRAPHICS Global variables: a camera, a checkerboard and some balls.
//KOL_sim::Checkerboard checkerboard(8, 8);
//KOL_sim::Camera camera;
//std::vector<KOL_sim::Ball> balls;
//
//
//KOL_sim::Graphics drawings = KOL_sim::Graphics(camera);









//END GRAPHICS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Simulate returns a vector of vectors. Index is the mass#.


//Open all the files for every ball,
//At every call to display, read the next line and return the postition


void makeLattice(std::vector<KOL_sim::Mass>& , std::vector<KOL_sim::Spring>&, bool = true);

int main(int argc, char** argv)
{
    KOL_sim::Simulation sim = KOL_sim::Simulation(T, DT);

    std::vector<KOL_sim::Mass> masses;
    std::vector<KOL_sim::Spring> springs;


    //Pendulum
    masses.push_back(KOL_sim::Mass(0.0,0,2, 1, 0.05, GREEN, true));
    masses.push_back(KOL_sim::Mass(1.9,0,2,100, 0.12, RED));
//    makeLattice(masses, springs, true);
    for (int i=0; i < masses.size()-1; ++i)
        for (int j = i + 1; j < masses.size(); ++j)
            springs.push_back(KOL_sim::Spring(&masses[i], &masses[j], 100'000));

    sim.simulate(springs, masses, argc, argv); //outputs position into a file.
    sim.start();
    return 0;

}




//    masses.push_back(KOL_sim::Mass(0,0,1, 0.8));
//    masses.push_back(KOL_sim::Mass(3,0,1, 0.8));
//
//    masses.push_back(KOL_sim::Mass(0.5*3,0.87*3,1, 0.8));
//    masses.push_back(KOL_sim::Mass(0.5*3,0.29*3,0.82*3+1, 0.8));

// CUBE
//     back 4
//    masses.push_back(KOL_sim::Mass(0,0,0, 1, 0.05, RED));
//    masses.push_back(KOL_sim::Mass(0,0,1, 1, 0.05, RED));
//
//    masses.push_back(KOL_sim::Mass(1,0,0, 1, 0.05, RED));
//    masses.push_back(KOL_sim::Mass(1,0,1, 1, 0.05, RED));
//    // front 4
//    masses.push_back(KOL_sim::Mass(0,1,0, 1, 0.05, RED));
//    masses.push_back(KOL_sim::Mass(0,1,1, 1, 0.05, RED));
//
//    masses.push_back(KOL_sim::Mass(1,1,0, 1, 0.05, RED));
//    masses.push_back(KOL_sim::Mass(1,1,1, 1, 0.05, RED));



// Tetrahidron
    // C (0.5, 0.29, 0.82)  A = (1,0,0), O (0,0,0), B (0.5, 0.87, 0)
//        masses.push_back(KOL_sim::Mass(0,0,1, 0.8));
//        masses.push_back(KOL_sim::Mass(3,0,1, 0.8));
//
//        masses.push_back(KOL_sim::Mass(0.5*3,0.87*3,1, 0.8));
//        masses.push_back(KOL_sim::Mass(0.5*3,0.29*3,0.82*2+1, 0.8));

//Pendulum
//            masses.push_back(KOL_sim::Mass(0.0,0,2, 1, 0.05, GREEN, true));
//            masses.push_back(KOL_sim::Mass(1.9,0,2,100, 0.12, RED));




void makeLattice(std::vector<KOL_sim::Mass>& masses, std::vector<KOL_sim::Spring>& springs, bool c) {
 
    // Lattice
    //    int coloring =0;
        for(int i=0; i<5; ++i) {
            for(int j=0; j<5; ++j) {
                for(int k=1; k<6; ++k) {
    //                if (coloring % 2 == 0)
                        masses.push_back(KOL_sim::Mass(j*0.2,i*0.2,k*0.2+3, 3, 0.07, RED));
    //                else
    //                    masses.push_back(KOL_sim::Mass(j*0.2,i*0.2,k*0.2+3, 1, 0.07, GREEN));
    //                coloring++;
                }
            }
        }
}



















void CreateLattice(const std::vector<double> & center, const std::vector<double> & dims, int nx, int ny, int nz, std::vector<KOL_sim::Mass> masses, std::vector<KOL_sim::Spring> springs) {


    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {


                KOL_sim::Mass m;
                double xd = (nx > 1) ? (double) i / (nx - 1.0) - 0.5 : 0;
                double yd =  (ny > 1) ? j / (ny - 1.0) - 0.5 : 0;
                double zd = (nz > 1) ? k / (nz - 1.0) - 0.5 : 0;

                std::vector<double> vec = {xd, yd, zd};

                for(int i=0; i<3; ++i) {
                    vec[i] *= dims[i];
                    vec[i] += center[i];
                }

                masses.push_back(KOL_sim::Mass(vec[0], vec[1], vec[2]));
            }
        }
    }

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                for (int l = 0; l < ((i != nx - 1) ? 2 : 1); l++) {
                    for (int m = 0; m < ((j != ny - 1) ? 2 : 1); m++) {
                        for (int n = 0; n < ((k != nz - 1) ? 2 : 1); n++) {
                            if (l != 0 || m != 0 || n != 0) {
                                springs.push_back(KOL_sim::Spring(&masses[k + j * nz + i * ny * nz],
                                                             &masses[(k + n) + (j + m) * nz + (i + l) * ny * nz], 10'000));
                            }
                        }
                    }
                }

                if (k != nz - 1) {
                    if (j != ny - 1) {
                        springs.push_back(KOL_sim::Spring(&masses[(k + 1) + j * nz + i * ny * nz], // get the full triangle
                                                     &masses[k + (j + 1) * nz + i * ny * nz], 10'000));
                    }

                    if (i != nx - 1) {
                        springs.push_back(KOL_sim::Spring(&masses[(k + 1) + j * nz + i * ny * nz],
                                                     &masses[k + j * nz + (i + 1) * ny * nz], 10'000));
                    }

                    if (j != ny - 1 && i != nx - 1) {
                        springs.push_back(KOL_sim::Spring(&masses[(k + 1) + j * nz + i * ny * nz],
                                                     &masses[k + (j + 1) * nz + (i + 1) * ny * nz], 10'000));
                        springs.push_back(KOL_sim::Spring(&masses[(k + 1) + j * nz + (i + 1) * ny * nz],
                                                     &masses[k + (j + 1) * nz + i * ny * nz], 10'000));
                        springs.push_back(KOL_sim::Spring(&masses[(k + 1) + (j + 1) * nz + i * ny * nz],
                                                     &masses[k + j * nz + (i + 1) * ny * nz], 10'000));
                    }
                }

                if (j != ny - 1 && i != nx - 1) {
                    springs.push_back(KOL_sim::Spring(&masses[k + (j + 1) * nz + i * ny * nz],
                                                 &masses[k + j * nz + (i + 1) * ny * nz], 10'000));
                }
            }
        }
    }

    for (KOL_sim::Spring & s : springs) {
        std::vector<double> temp;
        for(int i=0; i<3; ++i)
            temp.push_back(s.getMass2().position()[i] - s.getMass1().position()[i]);

        s.setRestLength(sqrt(pow(temp[0], 2) + pow(temp[1], 2) + pow(temp[2], 2)));
    }
}
