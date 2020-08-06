// Created by N.R. Stillman & S. Henkes 2020
//
#include "Domain.h"
#include <cmath>
#include <iterator>

#define _USE_MATH_DEFINES

// Domain constructor
Domain::Domain(Parameters params){
    cutoff = params.cutoff;
    cutoffZ = params.cutoffZ;
    maxmove = params.maxmove;
    Lx = params.Lx;
    Ly = params.Ly;
    if (params.bc_opt == "periodic"){periodic = true;}
    std::cout << "Initialised Domain" << std::endl;
}

// vector between two particles
std::array<double,2> Domain::calc_dr(std::array<double,2> xi, std::array<double,2> xj)
{
    double x = xj[0] - xi[0];
    double y = xj[1] - xi[1];
    if (periodic){
        if (x>Lx/2){x-=Lx;}
        else if (x<-Lx/2){x+=Lx;}
        else if (y>Ly/2){y-=Ly;}
        else if (y<-Ly/2){y+=Ly;}
    }
    return {x, y};
}

// distance between two particles
double Domain::dist(std::array<double,2> i, std::array<double,2> j)
{
    std::array<double,2> dr = calc_dr(i,j);
    return sqrt(dr[0]*dr[0] + dr[1]*dr[1]);
}

// compute the actual number of neighbours here, based on the interaction range
// The cutoffZ here is *mandatorily* smaller than the neighbour list cutoff
// This is now done with each interaction calculation
int Domain::countZ(std::vector<std::shared_ptr<Particle>> particles, int i) {
    int z = 0;
    // get the particles which are in the local neighbour list
    std::list <std::shared_ptr<Particle>> neighs = NeighbourList[i];
    for (auto n : neighs) {
            double dist_ip = dist(particles[i]->getPosition(), n->getPosition());
            if (dist_ip <cutoffZ) z+=1;
    }
    return z;
}

// create the neighbour list
// gets passed all the currently existing particles
// and a suitable cutoff, which is *larger* than the maximum existing interaction range,
// optimal value is in the range of the first maximum of g(r), about 1.4 interaction ranges
int Domain::makeNeighbourList(std::vector<std::shared_ptr<Particle>> particles){
    NeighbourList.clear();
    //vector of previous positions of particle (used in rebuild)
    auto p = particles.begin();
    std::advance(p, boundarysize);
    int idx = 0;
    while (p != particles.end()) {

        (*p)->setIndex(idx);
        (*p)->setPrevPosition();

        std::list<std::shared_ptr<Particle>> pneighs;
        int numneighs = 0;
        std::vector<std::shared_ptr<Particle>>::iterator neigh, end;
        for(neigh = particles.begin(), end = particles.end() ; neigh != end; ++neigh) {
        if ((*p)->getId() != (*neigh)->getId() ){
            double dist_pq = dist((*p)->getPosition(), (*neigh)->getPosition());
            if (dist_pq < cutoff) {
                pneighs.push_back((*neigh));
                numneighs += 1;
                }
            }
        }
        NeighbourList.push_back(pneighs);
        (*p)->setNumNeigh(numneighs);
		std::cout << "particle " << idx << " with id " << (*p)->getId() << " has " << numneighs << " in the neighbour list " << std::endl;
        idxmap[(*p)->getId()] = idx + boundarysize;

        pneighs.clear();
        ++p;
        idx += 1;
    }
    return 1;
}

// check for a neighbour list rebuild based on max motion of particles
bool Domain::checkRebuild(std::vector<std::shared_ptr<Particle>> particles) {
    for  (int i = boundarysize; i< particles.size(); ++i)  {
        // this is not pretty
        std::array<double,2> drmove = calc_dr(particles[i]->getPrevPosition(), particles[i]->getPosition());
        double distmove = sqrt(drmove[0]*drmove[0] + drmove[1]*drmove[1]);
        if (distmove > maxmove) return true;
    }
    return false;
}

// return the list of neighbours of particle i
// cannot be used to get boundary cell neighbours (which aren't stored)
std::list<std::shared_ptr<Particle>> Domain::getNeighbours(int i) {
    return NeighbourList[i];
}
