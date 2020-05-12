//
// Silke Henkes 26.04.2020

#ifndef CAPMD_DYNAMICS_H
#define CAPMD_DYNAMICS_H

#include "Particle.h"
#include "Parameters.h"

// handles self-propulsion and overdamped motion
class Dynamics {

    private:
        // dynamics parameters
        Parameters params;

        // magnitude of the active force
        std::vector <double> factive;
        // substrate friction
        std::vector <double>  zeta;
        // correlation time of the active driving
        std::vector <double>  tau;
    public:
        //Constructor
        Dynamics();

        // also needs a random number generator for the angular dynamics
        int rng;

        // move a particle according to the force law, and add active motion
        void step(Particle &, double);
};

#endif //CAPMD_DYNAMICS_H
