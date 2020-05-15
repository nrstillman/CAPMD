#include "Dynamics.h"

#include <cmath>
#include <random>

#include <iostream>

#define _USE_MATH_DEFINES

Dynamics::Dynamics() {
    // set active parameters here
    factive = params.factive;
    zeta = params.zeta;
    tau = params.tau;
}

// move a particle according to the force law, and add active motion
void Dynamics::step(Particle &p, double dt) {

    std::random_device rd;
    std::mt19937 gen(rd());

    //TODO: check rng
    std::normal_distribution<double> rng(0.0, 1);

    // get particle posn
    std::vector<double> x = p.getPosition();

    // compute the active force, according to its current direction along a unit vector that makes an angle theta with the x-axis
    std::vector<double> unit = {cos(p.theta),sin(p.theta)};
    std::vector <double> factvector = {factive[p.type]*unit[0], factive[p.type]*unit[1]};

    // update the positions, according to Euler in the simplest approach
    // Why not something more sophisticated? The angular, stochastic, step is much more complex otherwise
    for (int i = 0; i<2; i++){
        x[i] += (factvector[i] + p.force[i])/zeta[p.type]*dt;
    }
    p.position = x;

//    std::cout << p.theta << std::endl;

    // update the angle. Here, in the simplest approach, there is no angular torque from either active or passive sources
    // note stochastic calculus: The rotational diffusion constant is 2/tau, but the noise strength is 2/tau*sqrt(dt)
    // multiply by random number chosen from a normal distribution with mean 0 and standard deviation 1
    p.theta += 2.0/tau[p.type]*sqrt(dt)*rng(gen);
//    std::cout << "random number " << rng(gen) << std::endl;
//    std::cout << "theta update " << 2.0/tau[p.type]*sqrt(dt)*rng(gen)<< std:: endl;
//    std::cout << "new theta "<< p.theta << std::endl;
//    std::cout << x[0] << ',' << x[1] << std::endl;

    /// more general form if some form of torque or alignment is present:
    // p.theta += alignmentTorque*dt + 2.0/tau[p.type]*sqrt(dt)*rng(gen);
}