//
// Created by N.R. Stillman on 2020-04-20.
//

#include "Simulation.h"
#include "Parameters.h"

int main() {
    Parameters params;
    Simulation sim = Simulation(params);

    std::cout << sim.popSize() << std::endl;

    int t_final = 1000;
    for (int i = 0; i<= t_final; i++){
        sim.move();
        if (i % 100 == 0){
            std::cout << "---------" << std::endl;
            std::cout << "timestep: "<< i << std::endl;
//            sim.saveVTP(i, t_final);
        }
    }
}

