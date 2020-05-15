//
// Created by N.R. Stillman on 2020-04-20.
//

#include <iostream>
#include <vector>
#include <list>

#include "Simulation.h"

int main() {
    Simulation sim = Simulation();

    sim.initialise();
    Particle p = sim.getParticle(0);
    std::vector<double> x = p.getPosition();

    std::cout<< x[0] << ", " << x[1] <<std::endl;

    std::list<int> n;
    n = sim.getNeighbours(p);
    sim.getPopulationPosition(n);

    for (int i = 0; i<6;i++){
        sim.move();
    }

    sim.getPopulationPosition(n);
    Particle p0 = sim.getParticle(0);
    x = p0.getPosition();

    std::cout<< x[0] << ", " << x[1] <<std::endl;

}

