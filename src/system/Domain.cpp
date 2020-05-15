#include "Domain.h"
#include <cmath>

#define _USE_MATH_DEFINES

// Domain constructor
Domain::Domain()
{
//    std::vector<std::list<int>> _NeighbourList = {};
//    std::vector<std::vector<double>> PrevPositions = {};
}
// vector between two particles
std::vector<double> Domain::calc_dr(std::vector<double>  xi, std::vector<double> xj)
{
    std::vector<double> dr;

    std::transform(xj.begin(), xj.end(), xi.begin(), std::back_inserter(dr), std::minus<double>());
    return dr;
}

// distance between two particles
double Domain::dist(Particle i, Particle j)
{
    std::vector<double> dr = calc_dr(i.position,j.position);
    double dist = sqrt(dr[0]*dr[0] + dr[1]*dr[1]);
    return dist;
}

// create the neighbour list
// gets passed all the currently existing particles
// and a suitable cutoff, which is *larger* than the maximum existing interaction range,
// optimal value is in the range of the first maximum of g(r), about 1.4 interaction ranges
void Domain::makeNeighbourList(std::vector<Particle> particles, int cutoff){
    std::vector<std::list<int>> _NeighbourList;
    //vector of previous positions of particle (used in rebuild)
    std::vector<std::vector<double>> _PrevPositions;

    for (auto p : particles){
        _PrevPositions.push_back(p.position);
        std::list<int> pneighs;
        for (auto q: particles) {
            if (p.getId() != q.getId()){
                double dist_pq = dist(p, q);

                if (dist_pq < cutoff) {
                    pneighs.push_back(q.getId());
                }
            }
        }
        _NeighbourList.push_back(pneighs);
        pneighs.clear();
    }
    NeighbourList = _NeighbourList;
    PrevPositions = _PrevPositions;
}

//// check for a neighbour list rebuild based on max motion of particles
bool Domain::checkRebuild(std::vector<Particle> particles, double maxmove) {
    for (Particle& p : particles) {
        // this is not pretty
        std::vector<double> drmove = calc_dr(PrevPositions[p.getId()], p.getPosition());
        double distmove = sqrt(drmove[0]*drmove[0] + drmove[1]*drmove[1]);
        if (distmove > maxmove) return true;
    }
    return false;
}