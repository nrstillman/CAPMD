// Created by N.R. Stillman & S. Henkes 2020

#include "Parameters.h"

#include "Interface.h"
#include "Simulation.h"
#include <chrono>
#include <vector>

int main() {

    int L = 60;
    int N = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++){

        Parameters params;
        params.Lx = L; params.Ly = L;
        params.N = N;
        params.filename = "MSD";
        params.ntypes = 2;
        params.deathrate = 0;
        params.divrate = 0;
        params.pairstiff =
                {
                        { 0, 0},
                        { 0, 0},
                };
        params.pairatt =
                {
                        { 0, 0},
                        { 0, 0},
                };

        params.factive = {0, 0.01};
        params.zeta = {0, 1};
        params.tau = {0, 10};

        Interface sim = Interface(params);

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

