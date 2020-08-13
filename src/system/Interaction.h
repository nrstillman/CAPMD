// Created by N.R. Stillman & S. Henkes 2020
//
#ifndef CAPMD_INTERACTION_H
#define CAPMD_INTERACTION_H

#include <array>
#include <memory>
#include <vector>

#include "Particle.h"
#include "Parameters.h"

// Stores and computes interactions between particles of different types
// Doing this without derived classes, as we are nailing down the interaction potential once and for all

class Interaction {

private:
    Parameters params;

    // types of particles that exist in the simulation
    int ntypes;
    bool periodic;
    double Lx, Ly;
    // what are their relative stiffnesses ... (k_ij)
    std::vector<std::vector<double>> pairstiff;
    // ... and their relative attraction strengths (epsilon_ij)
    std::vector<std::vector<double>> pairatt;

    // fade-in (or out) time for particle interactions
    double fade;

public:
    Interaction(Parameters);

    // vector between two particles
    std::array<double,2> calc_dr(std::array<double,2>, std::array<double,2> );

    // distance between two particles or doubles
    double dist(std::array<double,2>, std::array<double,2> );


    // compute the mechanical force between particles
    void computeForce(std::shared_ptr<Particle>, std::shared_ptr<Particle>);
};

#endif //CAPMD_INTERACTION_H