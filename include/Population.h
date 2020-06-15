
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
    int divrates;
    // death rate
    int deathrates;
    // maximum density at division (in units of particle neighbours)
    int cutoffZ;
    int seed;

public:
        Population(Parameters);
        bool testDivide(int, int, double);

        bool testDeath(int, double);

};


#endif //CAPMD_POPULATION_H