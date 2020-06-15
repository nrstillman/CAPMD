
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>

#include "Population.h"

// Population constructor
Population::Population(){
}

Population::Population(Parameters params) {
// read out number of types, division and death rates

    ntypes = params.ntypes;
// type-specific division rates
    divrates = params.divrate;
// type-specific death rates
    deathrates = params.deathrate;
    // maximum density at division (in units of particle neighbours)
    cutoffZ = params.cutoffZ;
    seed = params.initseed;
// it has its own random number generator
//    RNG rng;
}


// check if the particle will divide
// takes a particle, a number of neighbours for it, and a random number generator
// the time interval is the amount of real time that has elapsed since the last check
bool Population::testDivide(int i, int z, double timeint) {


    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(seed);

    std::uniform_real_distribution<> rng(0.0, 1);

    /// i is for referencing specific type
    // actual density-dependent division rate
    double divreal = divrates*(1-z/cutoffZ);
    // include timeinterval to make this a division probability.
    // Note that the resulting number needs to remain << 1 for accuracy
    divreal = divreal*timeint;
    // final check on division: If divreal is larger than a randomly chosen number in (0,1)
    // divid, else don't
    if (divreal > rng(gen)) return true; else return false;
}

// check if a particle will die. Similar to division, just without the z-dependence (which can be added if desired)
bool Population::testDeath(int i, double timeint) {

    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(seed);

    std::uniform_real_distribution<> rng(0.0, 1);

    // include timeinterval to make this a division probability.
    // Note that the resulting number needs to remain << 1 for accuracy
    double deathreal = divrates*timeint;
    // final check on death: If deathreal is larger than a randomly chosen number in (0,1)
    // divid, else don't
    if (deathreal > rng(gen)) return true; else return false;
}
