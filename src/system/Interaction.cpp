// Created by N.R. Stillman & S. Henkes 2020
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

// vector between two particles
std::vector<double> Interaction::calc_dr(std::vector<double> xi, std::vector<double> xj)
{
    return {xj[0] - xi[0], xj[1] - xi[1]};
}

// distance between two particles
double Interaction::dist(std::vector<double> i, std::vector<double> j)
{
    return sqrt((j[0] - i[0])*(j[0] - i[0]) + (j[1] - i[1])*(j[1] - i[1]));
}

void Interaction::computeForce(std::shared_ptr<Particle> i, std::shared_ptr<Particle> j) {

    // get pair parameters
    double kij = pairstiff[i->getType()][j->getType()];
    double eps = pairatt[i->getType()][j->getType()];

    // compute vector distance between particles
    std::vector<double> dr = calc_dr(i->getPosition(),j->getPosition());
    // compute distance
    double dx = dist(i->getPosition(), j->getPosition());

    // actual force computation according to our potential
    // several lines since piecewise defined
    std::vector<double> force = {0,0};
    double bij = i->getRadius()+ j->getRadius();
    if (dx < bij*(1 + eps)) {
        force = {-kij*(bij - dx)*dr[0]/dx,-kij*(bij - dx)*dr[1]/dx};
    }
    else if(dx < bij*(1 + 2*eps)){
        force = {kij*(bij - dx - 2*eps)*dr[0]/dx, kij*(bij - dx - 2*eps)*dr[1]/dx};
    }
    // add force
    i->setForce({force[0] + i->getForce()[0], force[1] + i->getForce()[1]});

//    // multiply resulting force by amount of fade-in required. Cumulative if both particles are fading
//    // used for particle fade-in post division.
//    double multi = 1.0;
//    if (i->age<fade) {multi = i->age/fade;}
//    double multj = 1.0;
//    if (j->age<fade) {multj = j->age/fade;}

//    double multiplier = multi*multj;
//    std::transform(force.begin(), force.end(), force.begin(), [&multiplier](auto& c){return c*multiplier;});
}
