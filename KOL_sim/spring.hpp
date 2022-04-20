//
//  spring.hpp
//  KOL_sim
//
//  Created by Omer Mustel on 4/9/22.
//

#ifndef spring_hpp
#define spring_hpp

#include "mass.hpp"
#include <cmath>
//#include <iostream>



namespace KOL_sim {


class Mass;

class Spring {
    
    double L0; //rest length of the spring
    double K; // spring constant
    double defLength;
    Mass* m1;
    Mass* m2;
    double _omega; // frequency of oscillation
    double _damping; // damping on the masse
    friend class Simulation;

public:

    Spring(){};
    
    Spring(Mass* m1, Mass* m2, double K_c = 1000) : m1(m1), m2(m2), K(K_c) {
//        this->m1 = m1;
//        this->m2 = m2;
//        this->K = K_c;
        this->L0 = sqrt(pow(this->m1->p[0] - this->m2->p[0], 2) + pow(this->m1->p[1] - this->m2->p[1], 2) + pow(this->m1->p[2] - this->m2->p[2], 2));
        defLength=L0;
    }
    
    void operator = (const Spring &S ) {
        this->m1 = S.m1;
        this->m2 = S.m2;
        this->K = S.K;
        this->L0 = S.L0;
    }
    
    double springForce() {
//        std::cout << m1.p[0] << " " <<  m2.p[0] << "\n";
//        std::cout << m1.p[1] << " " <<  m2.p[1] << "\n";
//        std::cout << m1.p[2] << " " << m2.p[2] << "\n\n";
        double powers = pow(this->m1->p[0] - this->m2->p[0], 2) + pow(this->m1->p[1] - this->m2->p[1], 2) + pow(this->m1->p[2] - this->m2->p[2], 2);
//        std::cout << powers << "\n";

//        if (!powers || isnan(powers)) return K*(0 - L0);
        double L = sqrt(powers);
//        std::cout << L << " " << L0 << " " << K * (L - L0) << "\n";
        return this->K * (this->L0 - L);
    }
    
    
    void actuateSpring(double theta) { L0 = defLength + sin(theta)*0.5; }
    
    void setMass1(Mass * mass1) { m1 = mass1; }
    void setMass2(Mass * mass2) { m2 = mass2; }
    
    Mass getMass1() { return * m1; }
    Mass getMass2() { return * m2; }
    
    void modifySpringConstant(double constant) { K = constant; }
    
    void setRestLength(double L) { L0 = L; }
    ~Spring() {}

};


} //KOL_sim namespace

#endif /* spring_hpp */
