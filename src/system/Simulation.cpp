// Created by N.R. Stillman & S. Henkes 2020
//
#include "Simulation.h"

//#define _USE_MATH_DEFINES
#define M_PI 3.141592

Simulation::Simulation(Parameters _params){

    params = _params;

    domain = std::make_shared<Domain>(params);
    interaction = std::make_shared<Interaction>(params);
    dynamics = std::make_shared<Dynamics>(params);

    // create population of boundary particles <- must happen first
    Simulation::initBoundary();
    // update boundary size
    domain->setBoundarySize(boundarysize);

    // create population of particles
    Simulation::initPopulation();
    // create the first neighbour list
    domain->makeNeighbourList(particles);

    output = std::make_shared<Output>(params, boundarysize, particles);

    currentflag = 1;
}

void Simulation::setParams(Parameters new_params){

    params = new_params;
}

// initialise the system, including wiping previous particle vector
void Simulation::initialise() {

    // check previous initialisaiton
    if (currentflag == 1){
        particles.clear();
    } // check if population is empty
    else if (params.N == 0){
        throw "No particles (N=0).";
    }

    // create population of boundary particles <- must happen first
    Simulation::initBoundary();

    // create population of particles
    Simulation::initPopulation();

    // create the first neighbour list
    domain->makeNeighbourList(particles);

    currentflag = 1;
}


void Simulation::initBoundary() {
    if (params.bc_opt == "bounded") {

        // RNG: draw from random initial conditions in [-L/2,L/2]x[-L/2,L/2] with random angles
        std::uniform_real_distribution<> disx(0.0, 1);
        std::uniform_real_distribution<> disy(0.0, 1);
        std::uniform_real_distribution<> disr(0.0, 1);
        std::uniform_real_distribution<> distheta(0.0, 1);

        for (int i = 0; i < 2; i++) {
            double b_rho = 1; // <- density of particle boundaries
            for (int n = 0; n <= params.Lx/params.R/b_rho; n++) {

                double x = -params.Lx/2 + n*params.R*b_rho;
                double y = (params.Ly/2)*pow(-1,i);

                // initialise  a unique pointer to a new particle
                std::shared_ptr<Particle> pntrP(new Particle(boundarysize, params.btype, {x, y}, theta, radius));
                // move into vector of pointers
                particles.push_back(std::move(pntrP));

                boundarysize +=1;
            }
            for (int n = 0; n <= params.Ly/params.R/b_rho; n++) {
                double x = (params.Lx/2)*pow(-1,i);
                double y = -(params.Ly/2) + n*params.R*b_rho;

                std::shared_ptr<Particle> pntrP(new Particle(boundarysize, params.btype, {x, y}, theta, radius));

                particles.push_back(std::move(pntrP));

                boundarysize +=1;
            }
        }
    }
    if (params.bc_opt == "input") {
//       TODO: Add in input from .txt and python
        throw "Cannot accept input yet";
    }
}

void Simulation::initPopulation() {
    if (params.init_opt == "random_unif") {

        std::random_device rd;
        std::mt19937 gen(rd());
        gen.seed(params.initseed);

        // RNG: draw from random initial conditions in [-L/2,L/2]x[-L/2,L/2] with random angles
        // (these could better be moved to class defn but only initialised once so leave here for now)
        std::uniform_real_distribution<> disx(0.0, 1);
        std::uniform_real_distribution<> disy(0.0, 1);
        std::uniform_real_distribution<> disr(0.0, 1);
        std::uniform_real_distribution<> distheta(0.0, 1);

        std::cout << boundarysize << std::endl;
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

            std::shared_ptr<Particle> pntrP(new Particle(i, ptype, {x, y}, theta, radius));
            particles.push_back(std::move(pntrP));
        }
    }
    if (params.init_opt == "input") {
//       TODO: Add in input from .txt and python
        throw "Cannot accept input yet";
    }
}
// time stepping of the simulation
    void Simulation::move()
    {
        // compute forces
        #pragma omp simd // vectorized operation (does this speed up?)
        for (int i = boundarysize; i< particles.size(); ++i) {

            particles[i]->setForce({0,0});

            // get neighbours of p out of neighbour list
            std::list<int> neighbours = domain->getNeighbours(i - boundarysize);

            for (auto n : neighbours) {
                // use interaction to compute force
                interaction->computeForce(particles[i],particles[n]);
            }
        }

        // using the forces, update the positions and angles
        #pragma omp simd // vectorized operation (does this speed up?)
        for (int i = boundarysize; i< particles.size(); ++i) {
            dynamics->step(particles[i], params.dt);
        }
        bool rebuild = domain->checkRebuild(particles);

        if (rebuild){
            domain->makeNeighbourList(particles);
        }
    }
    // population dynamics here
    // For a density dependent divison rate:
    // First division, then death, else new particles will appear in the just vacated holes ...
    void Simulation::populationDynamics(int Ndiv) {
        // check for divisions
        // modify the particles list only after everybody has been checked
        std::vector<Particle> addparticles;
        //
//////TODO: Check below -> change to append directly to neighbour list
//        for (auto p : particles) {
//                // compute number of contacting neighbours
//                z = space.countZ(p,cutoffZ)
//                // and actual time elapsed since last division check
//                timeint = Ndiv*dt
//                // compute probabilistic chance at division
//                bool divide = population.testDivide(p,z,timeint)
//                if divide {
//                    // create a new particle in the same spot
//                    // index will be set by the neighbour list rebuild
//                    // flag is set here
//                    currentflag +=1
//                    // note: setting the new particle to the same radius as the old will
//                    // have unintended consequences (selecting for large particles)
//                    double radius = inicond.R*(1+poly*(rng.uniform(0,1)-0.5))
//                    // also, random orientation for similar reasons
//                    double theta = rng.uniform(0,1)*2*pi
//                    pnew = Particle(0,currentflag,p.type,p.position,theta,radius)
//                    // add to the list of new particles
//                    addparticles.append(pnew)
//                    // the clock of the old particle needs to be set back
//                    // This also triggers the fade-in effect between both
//                    p.age = 0.0
//                }
//        }
//        // now, at the end, stick the new particles at the end of the existing particle list
//        particles.extend(addparticles)
        // and rebuild the neighbour list
        domain->makeNeighbourList(particles);

        // Now, separately, we will check for death
        std::vector<int> deleteparticles;
        for (auto p : particles) {
                // compute actual time elapsed since last division check
                int timeint = Ndiv*params.dt;
                // TODO: compute probabilistic chance at division
//                bool death = population.testDeath(p,timeint)
//                if death {
//                    deleteparticles.append(p.getId())
//                };
        }
        // now, actually get rid of them
        // TODO: include
        std::sort(deleteparticles.begin(), deleteparticles.end());  // Make sure the container is sorted
        for (auto i = deleteparticles.rbegin(); i != deleteparticles.rend(); ++ i)
        {
            particles.erase(particles.begin() + *i);
        }
//Old way
//        for (auto pdel : deletparticles){
//                // throw out of particle list, addressed by pointer
//                particles.erase(pdel)
//                // delete the thing itself
//                pdel.deleteParticle()
//        }
        // and do a neighbour list rebuild to get all the indices straightened out again
        domain->makeNeighbourList(particles);
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


