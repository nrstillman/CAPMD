#include "Domain.h"
#include <cmath>

#define _USE_MATH_DEFINES

// Domain constructor
Domain::Domain()
{
}
// vector between two particles
std::vector<double> Domain::calc_dr(Particle i, Particle j)
{
    std::vector<double> dr;
    std::vector<double> a = i.position;
    std::vector<double> b = j.position;

    std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(dr), std::minus<double>());
    return dr;
}

// distance between two particles
double Domain::dist(Particle i, Particle j)
{
    std::vector<double> dr = calc_dr(i,j);
    double dist = sqrt(dr[0]*dr[0] + dr[1]*dr[1]);
    return dist;
}

// create the neighbour list
// gets passed all the currently existing particles
// and a suitable cutoff, which is *larger* than the maximum existing interaction range,
// optimal value is in the range of the first maximum of g(r), about 1.4 interaction ranges
std::vector<std::list<int>> Domain::makeNeighbourList(std::vector<Particle> particles, int cutoff){
    std::vector<std::list<int>> NeighbourList;
    for (auto p : particles){
        std::list<int> pneighs;
        for (auto q: particles) {
            if (p.getId() != q.getId()){
                double dist_pq = dist(p, q);
                if (dist_pq < cutoff) {
                    pneighs.push_back(q.getId());
                }
            }
        }
        NeighbourList.push_back(pneighs);
        pneighs.clear();
    }
    return NeighbourList;
}

//// check for a neighbour list rebuild based on max motion of particles
//bool checkRebuild(Particle* particles, double maxmove) {
//    for (auto p : particles){
//        // this is not pretty
//            drmove = p.position - positions0[p.index];
//            distmove = sqrt(drmove[0]**2 + drmove[1]**2);
//            if (distmove > maxmove) return True;
//    }
//    return False;
//}