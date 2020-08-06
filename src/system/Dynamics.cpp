// Created by N.R. Stillman & S. Henkes 2020
//
#include "Dynamics.h"
#include <cmath>
#include <iostream>
#include <random>

#define _USE_MATH_DEFINES

Dynamics::Dynamics(Parameters params) {
    // set active parameters here
    factive = params.factive;
    zeta = params.zeta;
    tau = params.tau;
    Lx = params.Lx;
    Ly = params.Ly;

    // change to periodic bc if chosen
    if (params.bc_opt == "periodic"){periodic = true;}

    gen = Engine(params.angseed);
    dist = Distribution(0,1);
}

// move a particle according to the force law, and add active motion
void Dynamics::step(std::shared_ptr<Particle> p, double dt) {

    double theta = p->getTheta();

    // get particle posn
    std::array<double,2> x = p->getPosition();
    std::array<double,2> f = p->getForce();

    // compute the active force, according to its current direction along a unit vector that makes an angle theta with the x-axis
    std::vector<double> unit = {cos(theta),sin(theta)};
    std::vector <double> factvector = {factive*unit[0], factive*unit[1]};

    // update the positions, according to Euler in the simplest approach
    // Why not something more sophisticated? The angular, stochastic, step is much more complex otherwise
    for (int i = 0; i<2; i++){
        x[i] += (factvector[i] + f[i])/zeta*dt;
    }
    if (periodic){
        if (x[0]>Lx/2){x[0]-=Lx;}
        else if (x[0]<-Lx/2){x[0]+=Lx;}
        else if (x[1]>Ly/2){x[1]-=Ly;}
        else if (x[1]<-Ly/2){x[1]+=Ly;}
    }
    p->setPosition(x);

    // update the angle. Here, in the simplest approach, there is no angular torque from either active or passive sources
    // note stochastic calculus: The rotational diffusion constant is 2/tau, but the noise strength is 2/tau*sqrt(dt)
    // multiply by random number chosen from a normal distribution with mean 0 and standard deviation 1
    theta += 2.0/tau*sqrt(dt)*dist(gen);
    p->setTheta(theta);
    p->setAge(p->getAge() + dt);

    /// more general form if some form of torque or alignment is present:
    // p.theta += alignmentTorque*dt + 2.0/tau[p.type]*sqrt(dt)*rng(gen);
}
