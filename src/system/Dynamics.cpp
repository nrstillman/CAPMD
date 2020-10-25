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
    alignmentTorque = params.alignmentTorque;

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

    // compute the active force, according to its current direction along a unit vector that makes an angle theta with the x-axis
    std::array<double, 2> factvector = {factive[p->getType()]*cos(theta), factive[p->getType()]*sin(theta)};

    // update the positions, according to Euler in the simplest approach
    // Why not something more sophisticated? The angular, stochastic, step is much more complex otherwise
    x[0] += (factvector[0]+ p->getForce()[0])/zeta[p->getType()]*dt;
    x[1] += (factvector[1] + p->getForce()[1])/zeta[p->getType()]*dt;

//    std::cout << "x = (" << x[0] << "," << x[1] << ")" << std::endl;
    if (periodic){
        if (x[0]>Lx/2){x[0]-=Lx;}
        else if (x[0]<-Lx/2){x[0]+=Lx;}
        else if (x[1]>Ly/2){x[1]-=Ly;}
        else if (x[1]<-Ly/2){x[1]+=Ly;}
    }
    p->setPosition(x);
    p->setActiveForce(factvector);
    p->setVel({(factvector[0] + p->getForce()[0])/zeta[p->getType()],
               (factvector[1] + p->getForce()[1])/zeta[p->getType()]});

    // update the angle. Here, in the simplest approach, there is no angular torque from either active or passive sources
    // note stochastic calculus: The rotational diffusion constant is 2/tau, but the noise strength is 2/tau*sqrt(dt)
    // multiply by random number chosen from a normal distribution with mean 0 and standard deviation 1
    theta += 2.0/tau[p->getType()]*sqrt(dt)*dist(gen);
    theta += alignmentTorque[p->getType()]*dt; //add in alignment (if it exists)

    p->setTheta(theta);
    p->setAge(p->getAge() + dt);
}
