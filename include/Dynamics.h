//
// Silke Henkes 26.04.2020

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
        double factive;
        // substrate friction
        double  zeta;
        // correlation time of the active driving
        double tau;

        std::random_device rd;
        typedef std::mt19937 Engine;
        typedef std::normal_distribution<double> Distribution;

        Engine gen;
        Distribution dist;

    public:
        //Constructor
        Dynamics(Parameters);

//        // also needs a random number generator for the angular dynamics
//        double rng;

        // move a particle according to the force law, and add active motion
        void step(std::shared_ptr<Particle> , double);
};

#endif //CAPMD_DYNAMICS_H
