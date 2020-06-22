// Created by N.R. Stillman & S. Henkes 2020

#include "Simulation.h"
#include "Parameters.h"
#include <chrono>
#include <vector>

int main() {


    int L = 20;
    int N = 30;

    auto start = std::chrono::high_resolution_clock::now();

    Parameters params;

    params.Lx = L;// Population size
    params.Ly = L;
    params.N = N; // Population size
    params.filename = "Run_w_params_N_" + std::to_string(params.N) + "L_ " + std::to_string(params.L);
    Simulation sim = Simulation(params);

    int t_final = 50000;
    for (int t = 0; t<= t_final; t++){
        sim.move();
        if (t % 1000 == 0){
            std::cout << "---------" << std::endl;
            std::cout << "timestep: "<< t << std::endl;
            sim.saveVTP(t, t_final);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

    cout << "Run_0: Time taken is " << duration.count() << endl;

}

