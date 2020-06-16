
//
#ifndef CAPMD_POPULATION_H
#define CAPMD_POPULATION_H

#include <vector>
#include <random>

#include "Parameters.h"
#include "Particle.h"

class Population{

private:
    // number of types
    int ntypes;
    // division rate
    double divrates;
    // death rate
    double deathrates;
    // maximum density at division (in units of particle neighbours)
    int maxZ;

    std::random_device rd;
    typedef std::mt19937 Engine;
    typedef std::uniform_real_distribution<double> Distribution;

    Engine gen;
    Distribution dist;

public:
        Population(Parameters);
        bool testDivide(int, double, double);

        bool testDeath(int, double);

};


#endif //CAPMD_POPULATION_H