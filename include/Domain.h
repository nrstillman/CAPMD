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
#include "Parameters.h"

// contains boundary conditions, and handles everything spatial
// essentially a container for NeighbourList, PrevPositions, etc.

class Domain
    {
    private:
        std::vector<std::list<int>> NeighbourList;
        std::vector<std::vector<double>> PrevPositions;

    public:

        double cutoff;
        int boundarysize;
        double maxmove;

        // Constructors
        Domain(Parameters);

        // apply periodic boundary conditions here if applicable
        // TODO: create boundary from parameters
        //  Domain(parameters.boundary);

        // vector between two particles
        std::vector<double> calc_dr(std::vector<double> , std::vector<double> );

        // distance between two particles or doubles
        double dist(std::shared_ptr<Particle>, std::shared_ptr<Particle>);
        double dist(double , double);

        void makeNeighbourList(std::vector<std::shared_ptr<Particle>>);
        bool checkRebuild(std::vector<std::shared_ptr<Particle>>);

        std::list<int> getNeighbours(int);

        void setBoundarySize( int x) { boundarysize = x;}
};



#endif //CAPMD_DOMAIN_H
