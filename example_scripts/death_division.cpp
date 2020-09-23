// Created by N.R. Stillman & S. Henkes 2020

#include "Parameters.h"

#include "Interface.h"
#include "Simulation.h"
#include <chrono>
#include <vector>

int main() {

    int L = 60;
    int N = 2;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++){

        Parameters params;
        params.Lx = L; params.Ly = L;
        params.N = N; // Population size
        params.filename = "death_division";
        params.deathrate = 0.01;
        params.divrate = 0.1;

        Interface sim = Interface(params);
        std::array<double, 2> x ;
        double Rlength = L/4.;
        std::array<double, 2> maxR = {Rlength, double(L)};
        std::array<double, 2> minR = {-Rlength, -1*double(L)};

        int t_final = params.t_final;
        for (int t = 0; t<= t_final; t++){
            sim.move();
            }
            if (t % params.output_time == 0) {
                sim.output->log(t);
                sim.saveData("text");
                sim.saveData("vtp");
            }
            if (t % params.popdynfreq == 0){sim.populationDynamics(100);}
        }
    }
}

