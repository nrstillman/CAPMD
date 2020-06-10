#include "Dynamics.h"

#include <cmath>
#include <random>

#include <iostream>

#define _USE_MATH_DEFINES

Dynamics::Dynamics(Parameters params) {
    // set active parameters here
    factive = params.factive;
    zeta = params.zeta;
    tau = params.tau;

    gen = Engine(params.angseed);
    dist = Distribution(0,1);

//    std::normal_distribution<double> theta_rng {0,1};
}

// pass by reference here
// move a particle according to the force law, and add active motion
void Dynamics::step(std::shared_ptr<Particle> p, double dt) {

    double theta = p->getTheta();

    // get particle posn
    std::vector<double> x = p->getPosition();
    std::vector<double> f = p->getForce();

    // compute the active force, according to its current direction along a unit vector that makes an angle theta with the x-axis
    std::vector<double> unit = {cos(theta),sin(theta)};
    std::vector <double> factvector = {factive*unit[0], factive*unit[1]};

    // update the positions, according to Euler in the simplest approach
    // Why not something more sophisticated? The angular, stochastic, step is much more complex otherwise
    for (int i = 0; i<2; i++){
        x[i] += (factvector[i] + f[i])/zeta*dt;
    }

    p->setPosition(x);

    std::vector<double> x2 = p->getPosition();

    // update the angle. Here, in the simplest approach, there is no angular torque from either active or passive sources
    // note stochastic calculus: The rotational diffusion constant is 2/tau, but the noise strength is 2/tau*sqrt(dt)
    // multiply by random number chosen from a normal distribution with mean 0 and standard deviation 1
//    double r = rng(gen);
    theta += 2.0/tau*sqrt(dt)*dist(gen);

    p->setTheta(theta);

    /// more general form if some form of torque or alignment is present:
    // p.theta += alignmentTorque*dt + 2.0/tau[p.type]*sqrt(dt)*rng(gen);
}