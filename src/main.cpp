//
// Created by N.R. Stillman on 2020-04-20.
//

#include "Simulation.h"
#include "Parameters.h"
#include <chrono>
#include <vector>

int main() {


    std::vector<int> L = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::vector<int> N = {30, 125, 285, 500, 800, 1150, 1560, 2000, 2500, 3200};

    for (int i = 0; i < 10; i++){
        auto start = std::chrono::high_resolution_clock::now();

        Parameters params;

        params.Lx = L[i];
        params.Ly = L[i];
        params.N = N[i]; // Population size
        params.filename = "Run_" + std::to_string(i) + "_N_" + std::to_string(params.N);
        Simulation sim = Simulation(params);

        int t_final = 50000;
        for (int t = 0; t<= t_final; t++){
            sim.move();
            if (t % 1000 == 0){
//                std::cout << "---------" << std::endl;
//                std::cout << "timestep: "<< t << std::endl;
                sim.saveVTP(t, t_final);
            }
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

        cout << "Run_" + std::to_string(i) + ": Time taken is " << duration.count() << endl;

    }

}

