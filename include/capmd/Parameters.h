// Created by N.R. Stillman & S. Henkes 2020

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
    std::string filename = "frame";
    std::string outputfolder = "./";

    // Random
    int initseed = 2022;
    int angseed = 2001;
    int popseed = 2021;

    // Simulation
    int popdynfreq = 100;
    int t_final = 10000;
    double dt  = 0.01; // fixed (upper bound)
    int dim = 2;
    double maxmove = 0.25;
    double cutoff = 3;
    double eps = 1E-3;

    //Domain
    // x and y length
    int Lx = 30; //30
    int Ly = 30; //30

    //Population
    int N = 1000; // Population size
    std::string init_opt = "random_unif"; //"random_unif";
    std::string bc_opt = "bounded";//"bounded";

    int NTA = 0;
    int Nstem = 0;
    int btype = 0;  // type for boundary cells

    //Particle
    int flag = 0;
    int type = 0;

    double R = 1;
    double poly = 0.3; // Matching [Silke, H, Soft Matt. 2016]

    //Interaction
    int ntypes = 3;
    double fade = 3;

    std::vector<std::vector<double>> pairstiff =
        {
            {10, 10, 10},
            {10, 1, 1},
            {10, 1, 1},
        };
    std::vector<std::vector<double>> pairatt =
        {
            { 0, 0, 0},
            { 0, 0.15, 0.15}, //max 0.2
            { 0, 0.15, 0.15}, //max 0.2
        };

    //Dynamics (active parameters)
    std::vector<double> factive = {0, 0.2, 0.001}; //active force
    std::vector<double> zeta = {0, 1, 1}; //friction
    std::vector<double> tau = {0, 10, 10}; //angular noise term

    // Division/death rates
    double deathrate = 0.005;
    double divrate = 0.1;

    double cutoffZ = 1 + 2*pairatt[0][0]; // (1 + 2*eps) - matching [Silke, H, Soft Matt. 2016]
    double maxZ = 6;

};

#endif //CAPMD_PARAMS_H
