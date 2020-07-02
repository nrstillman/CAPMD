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
    std::cout << "Initialised Domain" << std::endl;
}

// vector between two particles
std::vector<double> Domain::calc_dr(std::vector<double> xi, std::vector<double> xj)
{
    return {xj[0] - xi[0], xj[1] - xi[1]};
}

// distance between two particles
double Domain::dist(std::vector<double> i, std::vector<double> j)
{
    std::vector<double> dr = calc_dr(i,j);
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
void Domain::makeNeighbourList(std::vector<std::shared_ptr<Particle>> particles){
//
//    std::cout << "Neighbour List Calculated" << std::endl;
    NeighbourList.clear();
    //vector of previous positions of particle (used in rebuild)
    PrevPositions.clear();

    for (int i = boundarysize; i< particles.size(); ++i) {

            PrevPositions.push_back(particles[i]->getPosition());
            std::list<std::shared_ptr<Particle>> pneighs;
            int numneighs = 0;
            for (int j = 0; j< particles.size(); ++j) {

                if (particles[i]->getId() != particles[j]->getId() ){
                    double dist_pq = dist(particles[i]->getPosition(), particles[j]->getPosition());

                    if (dist_pq < cutoff) {
                        pneighs.push_back(particles[j]);
                        numneighs += 1;
                    }
                }
            }
            NeighbourList.push_back(pneighs);

            particles[i]->setNumNeigh(numneighs);
//            particles[i]->setZ(z);
            pneighs.clear();
        }
}

//// check for a neighbour list rebuild based on max motion of particles
bool Domain::checkRebuild(std::vector<std::shared_ptr<Particle>> particles) {
    for  (int i = boundarysize; i< particles.size(); ++i)  {
        // this is not pretty
        std::vector<double> drmove = calc_dr(PrevPositions[particles[i-boundarysize]->getId()], particles[i]->getPosition());
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