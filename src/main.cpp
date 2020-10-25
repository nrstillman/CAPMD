// Created by N.R. Stillman & S. Henkes 2020

#include "Parameters.h"

#include "Interface.h"
#include "Simulation.h"
#include <chrono>
#include <vector>

int main() {

    int L = 60;//, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int N = 200;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++){

        Parameters params;
        params.Lx = L*2; params.Ly = L;
        params.N = N; // Population size
        params.filename = "bounded_no_death";

        Interface sim = Interface(params);
        std::array<double, 2> x ;
        double Rlength = L/4.;
        std::array<double, 2> maxR = {Rlength, double(L)};
        std::array<double, 2> minR = {-Rlength, -1*double(L)};

        params.t_final = 20001;
        params.output_time = 50;
        int t_zap = 5000;
        for (int t = 0; t<= params.t_final; t++){
            sim.move();
            if (t == t_zap){
                int pop = sim.popSize();
                std::vector<int> popidx;
                for (int i = sim.boundarysize; i < sim.boundarysize + pop; i++) {popidx.push_back(i);}
                std::vector<int> popId = sim.getPopulationId(popidx);
                std::vector<std::array<double,2>> popPosn = sim.getPopulationPosition(popidx);
                std::vector<int> zapList;
                for (int i = 0; i < pop; i++){
                    std::array<double,2> x = popPosn[i];
                    if ((x[0] < maxR[0]) && (x[0] > minR[0])){
                        if ((x[1] < maxR[1]) && (x[1] > minR[1])){
                            zapList.push_back(popId[i]);
                        }
                    }
                }
//                sim.setCellTypes(zapList, 2);
                sim.killCells(zapList);

                std::cout << "Cell zapping stage completed" << std::endl;
            }
            if (t % params.output_time == 0) {
                //sim.updateOutput();
                sim.output->log(t);
//                sim.saveData("text");
                sim.saveData("vtp");
            }
            if (t % params.popdynfreq == 0){sim.populationDynamics(100);}
        }
    }
}

