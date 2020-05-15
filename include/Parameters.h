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
    int initseed = 2020;
    int angseed = 2001;
    int deathseed = 2021;

    // Simulation
    double dt  = 0.01;
    int dim = 2;
    int maxmove = 1;
    double cutoff = 2.5;
    double cutoffZ = 2.5;

    //Domain
    // x and y length
    int Lx = 50;
    int Ly = 50;

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
    double poly = 0.3;

    //Interaction
    int ntypes = 3;
    int fade = 0;
    std::vector<double> pairstiff  = {1,1};
    std::vector<double> pairatt = {0.2, 0.2}; //max limit

    //Dynamics (active parameters)
    std::vector<double> factive = {10};
    std::vector <double> zeta  = {1};
    double tau = 0.1;

};

#endif //CAPMD_PARAMS_H
