//
// Created by N.R. Stillman on 2020-04-20.
//

#include "Simulation.h"
#include "Output.h"

int main() {

    Simulation sim = Simulation();
    Simulation *ptrSim;

    ptrSim = &sim;

    sim.initialise();
    std::cout << sim.popSize() << std::endl;

    Particle p = sim.getParticle(0);

    std::cout << p.getId() << std::endl;

    Particle p1(2, 2, std::vector<double> {5,2}, 2, 2);
    p1.setPosition( std::vector<double>  {4,10});
    std::vector<double> x = p1.getPosition();
    std::cout<< x[0] << ", " << x[1] <<std::endl;

    std::list<int> n = sim.getNeighbours(p);
    std::cout << n.size() << std::endl;

//    sim.getPopulationPosition(n);

    Output out = Output(ptrSim);
    int t_final = 1;
    for (int i = 0; i<= t_final; i++){
        std::cout << i << std::endl;
        sim.move();
        std::cout << "---------" << std::endl;
//
//        if (i % 10 == 0){
        out.vtp(i, t_final);
//        }
    }
}

