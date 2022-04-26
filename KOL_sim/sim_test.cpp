#include "sim.hpp"

int main(int argc, char** argv) {
    // create the simulation object
    double DT = 0.0005; //time step
    double T = 14; //total time of simulation
    KOL_sim::Simulation sim = KOL_sim::Simulation(T, DT);

    //masses and spring vectors to store object structure
    std::vector<KOL_sim::Mass> masses;
    std::vector<KOL_sim::Spring> springs;
    
    //make an a pendulum
    KOL_sim::Object_Examples::create_pendulum(masses,springs);
    
    //pass the structure to sim and run
    sim.simulate(springs, masses); // run headless
    sim.start(argc, argv);         // run for visualization
    return 0;
}
