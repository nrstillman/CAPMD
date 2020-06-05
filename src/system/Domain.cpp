#include "Domain.h"
#include <cmath>
#include <iterator>


#define _USE_MATH_DEFINES

// Domain constructor
Domain::Domain(Parameters params){
    cutoff = params.cutoff;
    maxmove = params.maxmove;
}

// vector between two particles
std::vector<double> Domain::calc_dr(std::vector<double>  xi, std::vector<double> xj)
{
    std::vector<double> dr;

    std::transform(xj.begin(), xj.end(), xi.begin(), std::back_inserter(dr), std::minus<double>());
    return dr;
}

// distance between two particles
double Domain::dist(std::shared_ptr<Particle> i, std::shared_ptr<Particle> j)
{
    std::vector<double> dr = calc_dr(i->position,j->position);
    double dist = sqrt(dr[0]*dr[0] + dr[1]*dr[1]);
    return dist;
}

// distance between two particles
double Domain::dist(double i, double j)
{
    double dr = j - i;
    double dist = sqrt(dr*dr);
    return dist;
}

// create the neighbour list
// gets passed all the currently existing particles
// and a suitable cutoff, which is *larger* than the maximum existing interaction range,
// optimal value is in the range of the first maximum of g(r), about 1.4 interaction ranges
void Domain::makeNeighbourList(std::vector<std::shared_ptr<Particle>> particles){

    std::vector<std::list<int>> _NeighbourList;
    //vector of previous positions of particle (used in rebuild)
    std::vector<std::vector<double>> _PrevPositions;

    for (int i = boundarysize; i< particles.size(); ++i) {

            _PrevPositions.push_back(particles[i]->getPosition());
            std::list<int> pneighs;
            int numneighs = 0;
            for (int j = 0; j< particles.size(); ++j) {

                if (particles[i]->getId() != particles[j]->getId() ){
                    double dist_pq = dist(particles[i], particles[j]);

                    if (dist_pq < cutoff) {
                        pneighs.push_back(particles[j]->getId());
                        numneighs += 1;
                    }
                }
            }
            _NeighbourList.push_back(pneighs);

            particles[i]->setNumNeigh(numneighs);
            pneighs.clear();
        }
    NeighbourList = _NeighbourList;
    PrevPositions = _PrevPositions;
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
std::list<int> Domain::getNeighbours(int i) {

    return NeighbourList[i];
}

