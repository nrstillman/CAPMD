// Created by N.R. Stillman & S. Henkes 2020

#ifndef CAPMD_DYNAMICS_H
#define CAPMD_DYNAMICS_H

#include "Particle.h"
#include "Parameters.h"

#include <memory>
#include <random>

// handles self-propulsion and overdamped motion
class Dynamics {

    private:
        // dynamics parameters
        Parameters params;

        // magnitude of the active force
        std::vector<double> factive;
        // substrate friction
        std::vector<double> zeta;
        // correlation time of the active driving
        std::vector<double> tau;

        // bool condition for periodic bc
        bool periodic = false;
        double Lx;
        double Ly;

        typedef std::mt19937 Engine;
        typedef std::normal_distribution<double> Distribution;

        Engine gen;
        Distribution dist;

    public:
        //Constructor
        Dynamics(Parameters);

        // move a particle according to the force law, and add active motion
        void step(std::shared_ptr<Particle> , double);
};

#endif //CAPMD_DYNAMICS_H
