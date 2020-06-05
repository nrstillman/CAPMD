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

    // io
    std::string filename = "test";
    std::string outputfolder = "vtp/";
    // Random
    int initseed = 2020;
    int angseed = 2001;
    int deathseed = 2021;

    // Simulation
    double dt  = 0.01; // fixed (upper bound)
    int dim = 2;
    int maxmove = 1;
    double cutoff = 2.5;
    double cutoffZ = 2.5;

    //Domain
    // x and y length
    int Lx = 30; //30
    int Ly = 30; //30

    //Population
    // Population size
    int N = 250;
    std::string init_opt = "random_unif";
    std::string bc_opt = "bounded";

    int NTA = 0;
    int Nstem = 0;
    int btype = 0;  // type for boundary cells

    //Particle
    int flag = 0;
    int type = 0;

    double R = 1;
    double poly = 0.3; // 0.3

    //Interaction
    int ntypes = 2;
    int fade = 0;

    std::vector<std::vector<double>> pairstiff =
        {
            {10, 10},
            {10, 1},
        };
    std::vector<std::vector<double>> pairatt =
        {
            { 0, 0},
            { 0, 0.2},
        };

    //Dynamics (active parameters)
    double factive = 0.1; //0.1
    double zeta  = 1; //1
    double tau = 10; //10

};

#endif //CAPMD_PARAMS_H
