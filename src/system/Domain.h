// Created by N.R. Stillman & S. Henkes 2020

#ifndef CAPMD_DOMAIN_H
#define CAPMD_DOMAIN_H

#include <algorithm>    // std::transform
#include <functional>   // std::minus
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <map> // for idx to id map

#include "Particle.h"
#include "Parameters.h"

// contains boundary conditions, and handles everything spatial
// essentially a container for NeighbourList, PrevPositions, etc.

class Domain
    {
    private:
        std::vector<std::list<std::shared_ptr<Particle>>> NeighbourList;
        std::vector<std::vector<double>> PrevPositions;
        std::map<int,int> idxmap;

    public:

        double cutoff;
        double cutoffZ;
        int boundarysize;
        double maxmove;
        double Lx, Ly;
        bool periodic;

        // Constructors
        Domain(Parameters);

        // vector between two particles
        std::vector<double> calc_dr(std::vector<double>, std::vector<double> );

        // distance between two particles or doubles
        double dist(std::vector<double>, std::vector<double> );

        int countZ(std::vector<std::shared_ptr<Particle>>, int);
        void makeNeighbourList(std::vector<std::shared_ptr<Particle>>);
        bool checkRebuild(std::vector<std::shared_ptr<Particle>>);

        std::list<std::shared_ptr<Particle>> getNeighbours(int);

        int getIdx(int x){ return idxmap[x];}

        void setBoundarySize( int x) { boundarysize = x;}
};



#endif //CAPMD_DOMAIN_H
