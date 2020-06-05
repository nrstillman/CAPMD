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

        std::vector<std::list<int>> NeighbourList;
        std::vector<std::vector<double>> PrevPositions;
        // Constructors
        Domain();

        // apply periodic boundary conditions here if applicable
        // TODO: create boundary from parameters
        //  Domain(parameters.boundary);

        // vector between two particles
        std::vector<double> calc_dr(std::vector<double> , std::vector<double> );

        // distance between two particles
        double dist (std::shared_ptr<Particle>, std::shared_ptr<Particle>);
        double dist(double , double);

        void makeNeighbourList(std::vector<std::shared_ptr<Particle>>, int, int);
        bool checkRebuild(std::vector<std::shared_ptr<Particle>>, double, int);
    };



#endif //CAPMD_DOMAIN_H
