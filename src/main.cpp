//
// Created by N.R. Stillman on 2020-04-20.
//

#include "Simulation.h"

int main() {

    Simulation sim = Simulation();
    sim.initialise();
    std::cout << sim.popSize() << std::endl;

    Particle p = sim.getParticle(0);

    Particle p0;
    std::cout << p0.getId() << std::endl;

    Particle p1(2, 2, std::vector<double> {5,2}, 2, 2);
    p1.setPosition( std::vector<double>  {4,10});
    std::vector<double> x = p1.getPosition();
    std::cout<< x[0] << ", " << x[1] <<std::endl;
//
//    std::list<int> n;
//    std::cout<< x[0] << ", " << x[1] <<std::endl;
//
//
//    n = sim.getNeighbours(p);
//    sim.getPopulationPosition(n);
//
//
//    for (int i = 0; i<1;i++){
//        sim.move();
//    }
//    sim.getPopulationPosition(n);
//    Particle p0 = sim.getParticle(0);
//    x = p0.getPosition();
//
//    std::cout<< x[0] << ", " << x[1] <<std::endl;

}

