// Created by N.R. Stillman & S. Henkes 2020

#include "Parameters.h"

#include "Interface.h"
#include "Simulation.h"
#include <chrono>
#include <vector>

int main() {

    int L = 60;//, 20, 30, 40, 50, 60, 70, 80, 90, 100};
//    std::vector<int> N = {10};//, 125, 285, 500, 800, 1150, 1560, 2000, 2500, 3200};
    int N = 1150;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++){

        Parameters params;
        params.Lx = L; params.Ly = L;
        params.N = N; // Population size
        params.filename = "MSD";

        Interface sim = Interface(params);

        int t_final = 1000000;
        for (int t = 0; t<= t_final; t++){
            sim.move(t);
//            if (t % 100 == 0){sim.populationDynamics(100);}
//            if (t == 15000){
//                N = sim.popSize();
//                for (int i = sim.boundarysize; i < sim.boundarysize + N/2; i++){
//                    sim.killCell(i);
//                    sim.setCellType(i, 0);
//                }
//            }
            if (t % 1000 == 0) {
                sim.output->log(t);
                sim.output->savePopulation("test.txt");
                sim.output->vtp(t, t_final);
            }
        }
    }
}

