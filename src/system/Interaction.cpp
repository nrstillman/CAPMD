//
//
#include "Interaction.h"
#include <cmath>

#define _USE_MATH_DEFINES

Interaction::Interaction(Parameters params){
    // read out number of types, pair stiffnesses and pair attractions
    ntypes = params.ntypes;
    pairstiff = params.pairstiff;
    pairatt= params.pairatt;
    fade = params.fade;
}

std::vector<double> Interaction::computeForce(std::shared_ptr<Particle> i, std::shared_ptr<Particle> j, Domain* D) {
    // get pair parameters
    std::vector<double> x = i->getPosition();

    double kij = pairstiff[i->getType()][j->getType()];
    double eps = pairatt[i->getType()][j->getType()];

    // compute vector distance between particles
    std::vector<double> dr = D->calc_dr(i->getPosition(),j->getPosition());
    // compute distance
    double dist = D->dist(i,j);

    // actual force computation according to our potential
    // several lines since piecewise defined
    std::vector<double> force;
    double bij = i->radius + j->radius;
    if (dist < bij*(1 + eps)) {
        for (int n = 0;n<2; n++){
        force.push_back(-kij*(bij - dist)*dr[n]/dist);
        }
    }
    else if(dist < bij*(1 + 2*eps)){
        for (int n = 0;n<2; n++){
        force.push_back(kij*(bij - dist - 2*eps)*dr[n]/dist);
        }
    }
    else{
        force = {0,0};
    }

    ///TODO: update force and pos with coordinates
    // add force
    std::vector<double> f = {force[0] + i->getForce()[0], force[1] + i->getForce()[1]};
    i->setForce(f);

//    // multiply resulting force by amount of fade-in required. Cumulative if both particles are fading
//    // used for particle fade-in post division.
//    double multi = 1.0;
//    if (i->age<fade) {multi = i->age/fade;}
//    double multj = 1.0;
//    if (j->age<fade) {multj = j->age/fade;}

//    double multiplier = multi*multj;
//    std::transform(force.begin(), force.end(), force.begin(), [&multiplier](auto& c){return c*multiplier;});

    // output is inter-particle force
    return force;
}
