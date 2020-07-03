// Created by N.R. Stillman & S. Henkes 2020
//
#include "Simulation.h"

//#define _USE_MATH_DEFINES
#define M_PI 3.141592


Simulation::Simulation(){

    gen = Engine(params.initseed);
    disx = Distribution(0,1);
    disy = Distribution(0,1);
    distheta = Distribution(0,1);
    disr = Distribution(0,1);

    domain = std::make_shared<Domain>(params);
    interaction = std::make_shared<Interaction>(params);
    dynamics = std::make_shared<Dynamics>(params);
    population = std::make_shared<Population>(params);

    // particle counter for divison and death.
    currentflag = 0;

    // create population of boundary particles <- must happen first
    Simulation::initBoundary();
    // update boundary size
    domain->setBoundarySize(boundarysize);

    // create population of particles
    Simulation::initPopulation();
    // create the first neighbour list
    domain->makeNeighbourList(particles);

    output = std::make_shared<Output>(params, boundarysize, particles);
}

Simulation::Simulation(Parameters _params){

    params = _params;

    gen = Engine(params.initseed);
    disx = Distribution(0,1);
    disy = Distribution(0,1);
    distheta = Distribution(0,1);
    disr = Distribution(0,1);

    domain = std::make_shared<Domain>(params);
    interaction = std::make_shared<Interaction>(params);
    dynamics = std::make_shared<Dynamics>(params);
    population = std::make_shared<Population>(params);

    // particle counter for divison and death.
    currentflag = 0;

    // create population of boundary particles <- must happen first
    Simulation::initBoundary();
    // update boundary size
    domain->setBoundarySize(boundarysize);

    // create population of particles
    Simulation::initPopulation();
    // create the first neighbour list
    domain->makeNeighbourList(particles);

    output = std::make_shared<Output>(params, boundarysize, particles);
}

void Simulation::setParams(Parameters new_params){

    params = new_params;
}

// initialise the system, including wiping previous particle vector
void Simulation::initialise() {

    if (params.N == 0){
        throw "No particles (N=0).";
    }

    currentflag = 0;

    // create population of boundary particles <- must happen first
    Simulation::initBoundary();

    // create population of particles
    Simulation::initPopulation();

    // create the first neighbour list
    domain->makeNeighbourList(particles);


}


void Simulation::initBoundary() {
    if (params.bc_opt == "bounded") {

        // RNG: draw from random initial conditions in [-L/2,L/2]x[-L/2,L/2] with random angles < in class defn

        for (int i = 0; i < 2; i++) {
            double b_rho = 1; // <- density of particle boundaries
            for (int n = 0; n <= params.Lx/params.R/b_rho; n++) {

                double x = -params.Lx/2. + n*params.R*b_rho;
                double y = (params.Ly/2.)*pow(-1,i);

                // initialise  a unique pointer to a new particle
                std::shared_ptr<Particle> pntrP(new Particle(boundarysize, params.btype, {x, y}, theta, radius));
                // move into vector of pointers
                particles.push_back(std::move(pntrP));

                boundarysize +=1;
            }
            for (int n = 0; n <= params.Ly/params.R/b_rho; n++) {
                double x = (params.Lx/2.)*pow(-1,i);
                double y = -(params.Ly/2.) + n*params.R*b_rho;

                std::shared_ptr<Particle> pntrP(new Particle(boundarysize, params.btype, {x, y}, theta, radius));

                particles.push_back(std::move(pntrP));

                boundarysize +=1;
            }
        }
        currentflag += boundarysize;
    }
    if (params.bc_opt == "input") {
//       TODO: Add in input from .txt and python
        throw "Cannot accept input yet";
    }
}

void Simulation::initPopulation() {
    if (params.init_opt == "random_unif") {

        std::cout << "N of boundary particles is " << boundarysize << std::endl;
        for (int i = boundarysize; i < boundarysize + params.N; i++) {
            // (subtracting radius to avoid initialising on boundary)
            double x = (disx(gen) - 0.5) * (params.Lx - params.R);
            double y = (disy(gen) - 0.5) * (params.Ly - params.R);
            double theta = distheta(gen) * 2 * M_PI;

            // radius chosen from a uniform distribution with mean R and polydispersity poly
            double radius = params.R * (1 + params.poly * (disr(gen) - 0.5));

            int ptype = 1;
            // assign types here, e.g. 1 is TA cell, 2 is stem cell, 3 is tracer (0 is boundary)
            if (i < params.NTA) {
                ptype = 2;
            } else if (i < (params.NTA + params.Nstem)) {
                ptype = 3;
            }
            std::shared_ptr<Particle> pntrP(new Particle(currentflag, ptype, {x, y}, theta, radius));
            particles.push_back(std::move(pntrP));
            currentflag += 1;
        }
    }
    if (params.init_opt == "input") {
//       TODO: Add in input from .txt and python
        throw "Cannot accept input yet";
    }
}
// time stepping of the simulation
    void Simulation::move(int t)
    {
        double timeint = t*params.dt; // possibly for adding to age...

        auto p = particles.begin();
        std::advance(p, boundarysize);

        while (p != particles.end()) {
            // zero force and number of contact neighbours for each timestep
            (*p)->setForce({0,0});
            (*p)->setZ(0);
            // get neighbours of p out of neighbour list
            std::list<std::shared_ptr<Particle>> neighbours = domain->getNeighbours((*p)->getId() - boundarysize);

            for (auto n : neighbours) {
                // use interaction to compute force
                interaction->computeForce((*p),n);
            }
            ++p; // onto the next particle
        }
        // using the forces, update the positions and angles
        for (int i = boundarysize; i< particles.size(); ++i) {
            dynamics->step(particles[i], params.dt);
        }
        bool rebuild = domain->checkRebuild(particles);

        if (rebuild) {
            domain->makeNeighbourList(particles);
        }
    }
    // population dynamics here
    // First division, then death, else new particles will appear in the just vacated holes ...
    void Simulation::populationDynamics(int Ndiv) {
        // check for divisions
        // modify the particles list only after everybody has been checked
        std::vector<std::shared_ptr<Particle>> newparticles;
        bool rebuild  = false;
        auto p = particles.begin();
        std::advance(p, boundarysize);

        while (p != particles.end()) {
                // actual time elapsed since last division check
                double timeint = Ndiv*params.dt;
                // compute probabilistic chance at division
                bool divide = population->testDivide((*p)->getType(),(*p)->getZ(),timeint);
                if (divide) {

                    // note: setting the new particle to the same radius as the old will
                    // have unintended consequences (selecting for large particles)
                    double radius = params.R * (1 + params.poly * (disr(gen) - 0.5));
                    // also, random orientation for similar reasons
                    double theta = distheta(gen) * 2 * M_PI;

                    std::vector<double> x =  {(*p)->getPosition()[0] + disx(gen)*params.eps, (*p)->getPosition()[1] + disx(gen)*params.eps};
                    // create a new particle in the same spot
                    std::shared_ptr<Particle> pntrP(new Particle(currentflag, (*p)->getType(), x,theta,radius));
                    // flag is set here
                    currentflag +=1;
                    // index will be set by the neighbour list rebuild

                    // add to the list of new particles
                    newparticles.push_back(std::move(pntrP));

                    // the clock of the old particle needs to be set back
                    // This also triggers the fade-in effect between both
                    (*p)->setAge(0.0);
                    rebuild = true;
                }
            ++p;
        }
        // now, at the end, stick the new particles at the end of the existing particle list
        particles.insert(particles.end(), newparticles.begin(), newparticles.end());

        // and rebuild the neighbour list
        if (rebuild) domain->makeNeighbourList(particles);

///TODO: Include death of particles
//        // Now, separately, we will check for death
//        p = particles.begin();
//        std::advance(p, boundarysize);
//        std::vector<std::shared_ptr<Particle>> deleteparticles;
//
//        while (p != particles.end()) {
//                // compute actual time elapsed since last division check
//                int timeint = Ndiv*params.dt;
//                bool death = population->testDeath((*p)->getType(),timeint);
//                if (death) {
//                    std::cout << "death" << std::endl;
//                    deleteparticles.push_back((*p));
//                }
//                ++p;
//        }
//        // now, actually get rid of them
//        std::sort(deleteparticles.begin(), deleteparticles.end());  // Make sure the container is sorted
//
//        // delete the particle
//        particles.erase(deleteparticles.begin(), deleteparticles.end());
//                // get rid of the the pointer in the particle list
////                particles.erase(pdel);
////        }
////        and do a neighbour list rebuild to get all the indices straightened out again
//        domain->makeNeighbourList(particles);
}


// function to get particle in simulation (indexing starts from any boundary cells)
Particle Simulation::getParticle(int i){
    if (i >= particles.size()){
        std::cout << "Error: index exceeds population size"  << std::endl;
        throw;
    }

    return *particles[boundarysize + i];
}

// function to get particle in simulation (indexing starts from any boundary cells)
Particle Simulation::getAllParticles(int i){
    if (i >= particles.size()){
        std::cout << "Error: index exceeds population size"  << std::endl;
        throw;
    }
    return *particles[i];
}

// return the population positions
std::vector<std::vector<double>> Simulation::getPopulationPosition(std::list<int> &index){

    std::vector<std::vector<double>> positions(0 , std::vector<double>(2));
    for (auto i : index) {
        Particle p = *particles[i];
        positions.push_back(p.getPosition());
    }
    return positions;
}

// return the boundary positions
std::vector<std::vector<double>> Simulation::getBoundaryPosition(){

    std::vector<std::vector<double>> positions(0 , std::vector<double>(2));
    for (int i = 0; i< boundarysize; ++i) {
        Particle p = *particles[i];
        positions.push_back(p.getPosition());
    }
    return positions;
}

std::vector<double> Simulation::getPopulationRadius(std::list<int> &index){

    std::vector<double> radii(0);
    for (auto i : index) {
        Particle p = *particles[i];
        radii.push_back(p.getRadius());
    }
    return radii;
}

void Simulation::saveVTP(int step, int finalstep)
{
    output->setParticles(particles);
    output->vtp(step, finalstep);
}


void Simulation::savePopulation(std::string filepath)
{
    std::filebuf fb;
    fb.open (filepath,std::ios::app);
    std::ostream out(&fb);
    out << particles.size();
    out << '\n';

    for (auto p : particles) {
        out << p;
    }
    fb.close();
}

//TODO: allow for preloaded population posn
void Simulation::loadPopulation(std::string filepath)
{
    std::cout << "offline" <<std::endl;

}


