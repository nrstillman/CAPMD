// Created by N.R. Stillman & S. Henkes 2020
//
#ifndef CAPMD_INTERACTION_H
#define CAPMD_INTERACTION_H

#include <array>
#include <memory>
#include <vector>

#include "Particle.h"
#include "Parameters.h"
/**
    \file Interaction.h
    Stores and computes interactions between particles of different types
*/
/*!
   Note, we are setting interaction potential as fixed here (rather than using derived classes)
*/
class Interaction {

    public:
        Interaction(Parameters);

        //! Calculates vector between two particles
        std::array<double,2> calc_dr(std::array<double,2>, std::array<double,2> );

        //! Calculates distance between two particles (or doubles)
        double dist(std::array<double,2>, std::array<double,2> );

        //! Computes the mechanical force between particles
        void computeForce(std::shared_ptr<Particle>, std::shared_ptr<Particle>);

    private:
        Parameters params;

        int ntypes; //! Number of different types of particles that exist in the simulation
        bool periodic; //!< Indicating whether boundary is periodic or not
        double Lx, Ly; //!< Boundary edges

        std::vector<std::vector<double>> pairstiff; //! Particle stiffnesses ... (k_ij)
        std::vector<std::vector<double>> pairatt; //! Particle attraction strengths (epsilon_ij)

        double fade; //! Fade-in (or out) time for particle interactions
};

#endif //CAPMD_INTERACTION_H
