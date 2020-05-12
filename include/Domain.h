// Silke Henkes 26.04.2020
// N. R. Stillman 29.04.2020


#ifndef CAPMD_DOMAIN_H
#define CAPMD_DOMAIN_H

#include <vector>
#include <list>
#include <iostream>
#include <algorithm>    // std::transform
#include <functional> // std::minus

#include "Particle.h"

// contains boundary conditions, and handles everything spatial
// essentially a container for Population, NeighbourList, etc.
// including the neighbourlist <-- have made separate class

class Domain
    {
    public:

        // Constructors
        Domain();

        // apply periodic boundary conditions here if applicable
        // TODO: create boundary from parameters
        //  Domain(parameters.boundary);

        // vector between two particles
        std::vector<double> calc_dr(Particle, Particle);

        // distance between two particles
        double dist(Particle , Particle);


        std::vector<std::list<int>> makeNeighbourList(std::vector<Particle>, int);

    };



#endif //CAPMD_DOMAIN_H
