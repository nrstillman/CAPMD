//
//
#include "Interaction.h"
#include <cmath>

#define _USE_MATH_DEFINES

Interaction::Interaction(){
    // read out number of types, pair stiffnesses and pair attractions
    ntypes = params.ntypes;
    pairstiff = params.pairstiff;
    pairatt= params.pairatt;
    fade= params.fade;
}

std::vector<double> Interaction::computeForce(Particle i,Particle j, Domain D) {
    // get pair parameters
    std::vector<double> kij = pairstiff;
    double epsij = pairatt[i.type*ntypes + j.type];

    // compute vector distance between particles
    std::vector<double> dr = D.calc_dr(i,j);

    // actual force computation according to our potential
    // several lines since piecewise defined
    std::vector<double> force;
    for (int n = 0;n<2; n++){
        force.push_back(kij[n]*(i.radius + j.radius - std::abs(dr[n]))*dr[n]/std::abs(dr[n]));
    }


    // multiply resulting force by amount of fade-in required. Cumulative if both particles are fading
    double multi = 1.0;
    if (i.age<fade) {multi = i.age/fade;}
    double multj = 1.0;
    if (j.age<fade) {multj = j.age/fade;}

    double multiplier = multi*multj;
    std::transform(force.begin(), force.end(), force.begin(), [&multiplier](auto& c){return c*multiplier;});

    // output is inter-particle force
    return force;
}
