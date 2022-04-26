//
//  mass.hpp
//  KOL_sim
//
//  Created by Omer Mustel on 4/9/22.
//

#ifndef mass_hpp
#define mass_hpp

#include <vector>
#include <cmath>
#include <iostream>
#include "graphics.hpp"


namespace KOL_sim {

class Mass {

    
    friend class Simulation;
    friend class Spring;
    double m;
    std::vector<double> p; //position
    std::vector<double> v; //velocity
    std::vector<double> a; //acceleration
    std::vector<double> F; //external forces
    
    bool fixed;
    
public:
    double radius;
    GLfloat *color;
    Mass() : m(1), p({0,0,0}), v({0,0,0}), a({0,0,0}), F({0,0,0}) {};
    
    Mass(double x, double y, double z, double mass=1, float r=0.05, GLfloat* c=nullptr, bool fix=false) : m(mass), radius(r), fixed(fix) {
        
        if (radius < 0) {
            std::cerr << "KOL_sim::ERROR::Mass.radius cannot be negative.\n";
            exit(0);
        }
        GLfloat def[] = {1,0,0};
        if (c != nullptr) {
            color = c;
        }
        else color = def;
        
        //set intial position
        this->p.push_back(x);
        this->p.push_back(y);
        this->p.push_back(z);

        // set velocity, acceleration & Froce to 0
        for (int i=0; i < 3; i ++) {
            this->v.push_back(0);
            this->a.push_back(0);
            this->F.push_back(0); // F = a*m
        }
        // intially, gravitiy is the acceleration
    }

    ~Mass(){}

    // assigment operator overload
    void operator = (const Mass &M ) {
        this->m = M.m;
        this->p = M.p;
        this->a = M.a;
        this->v = M.v;
        this->F = M.F;
    }

    // calculate force
    void init_force(int x, int y, int z) {
        this->F.push_back(x*this->m);
        this->F.push_back(y*this->m);
        this->F.push_back(z*this->m);
    }

    // update velocity
    void update_velocity(const double DT) {
        if (!fixed) {
            for (int i = 0; i < 3; i++ ) {
                this->v[i] = this->v[i] + DT * this->a[i];
            }
        }
    }

    // update position
    void update_position(const double DT) {
        if (!fixed) {
            for (int i = 0; i < 3; i++ )
                this->p[i] = this->p[i] + DT*this->v[i];
        }
    }

    // calculate
    void update_acceleration() {
        if (!fixed) {
            for (int i = 0; i < 3; i++ )
                this->a[i] = this->F[i]/this->m;
        }
    }

    void update_damping(double dumping_constant=0.9999) {
        if (!fixed) {
            for(int i=0; i < 3; ++i)
                this->v[i] *= dumping_constant;
        }
    }
    
    void set_external_force(const std::vector<double> & v) { if (!fixed) F = v; }
    
    void add_external_force(const std::vector<double> & v) {
        if (!fixed){
            F[0] += v[0];
            F[1] += v[1];
            F[2] += v[2];
        }
    }
    
    void set_drag(double C) {  F[0] *= -C;  F[1] *= -C; F[2] *= -C; }
    
    std::vector<double> position() { return p; }
        
    void print() {
        std::cout << "Mass " << m << "\n";
        for(auto& f : p)
            std::cout << "position: " << f << "\n";
        for(auto& f : v)
            std::cout << "velocity: " << f << "\n";
        for(auto& f : a)
            std::cout << "acceleration: " << f << "\n";
        for(auto& f : F)
            std::cout << "External Forces: " << f << "\n";
    }
    
}; //Mass class



} //KOL_sim namespace

#endif /* mass_hpp */
