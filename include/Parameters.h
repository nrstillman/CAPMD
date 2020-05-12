#ifndef CAPMD_PARAMS_H
#define CAPMD_PARAMS_H

#include <string>
#include <vector>

struct Parameters {

    // everything relevant, including
    // information about system size
    // dynamical parameters
    // division death parameters
    // interaction parameters

    // Random
    int seed = 2020;

    // Simulation
    double dt  = 1;
    int dim = 2;
    int maxmove = 10;
    double cutoff = 1;
    int cutoffZ = 10;

    //Domain
    // x and y length
    int Lx = 20;
    int Ly = 20;

    //Population
    // Population size
    int N = 50;
    std::string init_opt = "random_unif";

    int NTA = 0;
    int Nstem = 0;

    //Particle
    int flag = 0;
    int type = 0;

    double R = 1;
    double poly = 1;

    //Interaction
    int ntypes = 3;
    int fade = 0;
    std::vector<double> pairstiff  = {0.4,0.4};
    std::vector<double> pairatt = {1};

    //Dynamics (active parameters)
    std::vector<double> factive = {1};
    std::vector <double> zeta  = {1};
    std::vector <double> tau= {1};

};

#endif //CAPMD_PARAMS_H
