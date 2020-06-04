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

    Particle p0;
    std::cout << p0.getId() << std::endl;

    Particle p1(2, 2, std::vector<double> {5,2}, 2, 2);
    p1.setPosition( std::vector<double>  {4,10});
    std::vector<double> x = p1.getPosition();
    std::cout<< x[0] << ", " << x[1] <<std::endl;

    std::list<int> n;

    n = sim.getNeighbours(p);
    sim.getPopulationPosition(n);
    Output out = Output(ptrSim);

    for (int i = 0; i<151;i++){

        sim.move();

        if (i % 50 == 0){
            std::cout << i << std::endl;
            out.vtp(i, 151);
        }
    }


}

