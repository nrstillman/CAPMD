//
//
#include "Parameters.h"

#include "Population.h"
#include "Domain.h"
#include "Dynamics.h"
#include "Interaction.h"
#include "Particle.h"
#include "Output.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <memory> // For std::shared_ptr


#ifndef CAPMD_SIMULATION_H
#define CAPMD_SIMULATION_H


// We tie it all together in the simulation
class Simulation : virtual public Particle{

    private:
        Parameters params;

        // As container, has the different sub-pieces
        std::shared_ptr<Domain> domain;
        std::shared_ptr<Dynamics> dynamics;
        std::shared_ptr<Interaction> interaction;
        std::shared_ptr<Output> output;

    public:

        std::vector<std::shared_ptr<Particle>> particles;

        // Members (taken from parameters but included incase changing through python script)
        // Number of particles
        int N;
        // Neighbour list (cutoff) and contact cutoff range (cutoffZ)
        double cutoff, cutoffZ;
        // time step
        double dt;
        // neighbour list total move threshold (should be of the order of 0.5)
        double maxmove;
        // running counter on particle flags <- using this to check initialisation instead
        int currentflag;
        // correlation time
        double tau;

        int boundarysize = 0;

        // Constructors:
//        Simulation();

        Simulation(Parameters);

        void setParams(Parameters);
        // Methods to initialise the system, including creating the particle vector and the first NeighbourList
        void initialise();
        void initPopulation(); //  To initialise a population
        void initBoundary(); //  To create the boundary

        // Methods for particle dynamics
        void move(void);
        void populationDynamics(int);

        // Methods for getting sim data
        int popSize(void){ return particles.size() - boundarysize;}
        int getN(void){ return particles.size();}
        int getBoundarySize(void){ return boundarysize;}
        std::string getFileName(){return params.filename;};


        // Methods for particle data
        Particle getParticle(int);
        Particle getAllParticles(int);
        std::vector<std::vector<double>> getPopulationPosition(std::list<int> &index);
        std::vector<std::vector<double>> getBoundaryPosition();
        std::vector<double> getPopulationRadius(std::list<int> &index);

        //  I/O for sim data
        void savePopulation(std::string);
        void loadPopulation(std::string);
        void saveVTP(int, int);

};


#endif //CAPMD_SIMULATION_H
