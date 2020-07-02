// Created by N.R. Stillman & S. Henkes 2020

#include "Simulation.h"
#include "Parameters.h"
#include <chrono>
#include <vector>

int main() {

    std::vector<int> L = {10};//, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::vector<int> N = {10};//, 125, 285, 500, 800, 1150, 1560, 2000, 2500, 3200};
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++){

        Parameters params;

        params.Lx = L[i];
        params.Ly = L[i];
        params.N = N[i]; // Population size
        params.filename = "Division_" + std::to_string(i) + "_N_" + std::to_string(params.N) +"_L_" + std::to_string(params.Lx) ;
        Simulation sim = Simulation(params);

        int t_final = 10000;
        for (int t = 0; t<= t_final; t++){
            sim.move(t);
            if (t % 100 == 0){
                sim.populationDynamics(100);
            }
            if (t % 100 == 0){
                std::cout << "---------" << std::endl;
                std::cout << "timestep: "<< t << std::endl;
                sim.saveVTP(t, t_final);
            }
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

    std::cout << "Run_0: Time taken is " << duration.count() << std::endl;

}

