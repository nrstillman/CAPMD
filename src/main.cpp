//
// Created by N.R. Stillman on 2020-04-20.
//

#include "Simulation.h"
#include "Output.h"
#include "Parameters.h"

int main() {
    Parameters params;
    Simulation sim = Simulation(params);

    std::cout << sim.popSize() << std::endl;

    int t_final = 250;
    for (int i = 0; i<= t_final; i++){
        std::cout << i << std::endl;
        sim.move();
        std::cout << "---------" << std::endl;

        sim.saveVTP(i, t_final);
    }
}

