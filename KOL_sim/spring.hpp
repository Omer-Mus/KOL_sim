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
    bool actuate_spr; //allow actuation
    friend class Simulation;

public:

    Spring(){};
    
    Spring(Mass* m1, Mass* m2, double K_c = 1000, bool actuate = false)
    : m1(m1), m2(m2), K(K_c), actuate_spr(actuate)
    {
        this->L0 = sqrt(pow(this->m1->p[0] - this->m2->p[0], 2) + pow(this->m1->p[1] - this->m2->p[1], 2) + pow(this->m1->p[2] - this->m2->p[2], 2));
        defLength=L0;
    }
    
    void operator = (const Spring &S ) {
        this->m1 = S.m1;
        this->m2 = S.m2;
        this->K = S.K;
        this->L0 = S.L0;
    }
    
    double spring_force() {
        double powers = pow(this->m1->p[0] - this->m2->p[0], 2) + pow(this->m1->p[1] - this->m2->p[1], 2) + pow(this->m1->p[2] - this->m2->p[2], 2);
//        std::cout << powers << "\n";

//        if (!powers || isnan(powers)) return K*(0 - L0);
        double L = sqrt(powers);
//        std::cout << L << " " << L0 << " " << K * (L - L0) << "\n";
        return this->K * (this->L0 - L);
    }
    
    
    void actuate_spring(double theta) { L0 = defLength + sin(theta)*0.5; }

    void set_mass1(Mass * mass1) { m1 = mass1; }
    void set_mass2(Mass * mass2) { m2 = mass2; }
    Mass * get_mass1() { return m1; }
    Mass * get_mass2() { return m2; }
    
    void modify_spring_constant(double constant) { K = constant; }
    
    void set_rest_length(double L) { L0 = L; }
    double get_rest_length() { return L0; }
    ~Spring() {}

};


} //KOL_sim namespace

#endif /* spring_hpp */
