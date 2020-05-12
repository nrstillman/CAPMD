//
// Silke Henkes 26.04.2020

#ifndef CAPMD_INTERACTION_H
#define CAPMD_INTERACTION_H

#include <vector>
#include <array>

#include "Particle.h"
#include "Parameters.h"
#include "Domain.h"

// Stores and computes interactions between particles of different types
// Doing this without derived classes, as we are nailing down the interaction potential once and for all

class Interaction {

private:
    Parameters params;

    // types of particles that exist in the simulation
    int ntypes;

    // what are their relative stiffnesses ... (k_ij)
    std::vector<double> pairstiff;
    // ... and their relative attraction strengths (epsilon_ij)
    std::vector <double> pairatt;
    // fade-in (or out) time for particle interactions
    double fade;

public:
    Interaction();

    // compute the mechanical force between particles
    std::vector<double> computeForce(Particle,Particle, Domain);
};

#endif //CAPMD_INTERACTION_H