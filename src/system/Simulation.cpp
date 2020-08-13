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
	// timestep
	timestep = 0;

    // create population of boundary particles <- must happen first
    if (params.bc_opt == "bounded") {Simulation::initBoundary();}

    // update boundary size
    domain->setBoundarySize(boundarysize);

    // create population of particles
    Simulation::initPopulation();
    // create the first neighbour list
    domain->makeNeighbourList(particles);

    output = std::make_shared<Output>(params, boundarysize, particles);
}

Simulation::Simulation(Parameters _params){
    std::cout << "Initialised Simulation w Parameters" << std::endl;

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
	// timestep
	timestep = 0;

    // create population of boundary particles <- must happen first
    if (params.bc_opt == "bounded") {Simulation::initBoundary();}
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
	// This has been done in the constructor already, needs a rethink
    if (params.bc_opt == "bounded") {Simulation::initBoundary();}

    // create population of particles
    Simulation::initPopulation();

    // create the first neighbour list
    domain->makeNeighbourList(particles);
}

void Simulation::initBoundary() {
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

void Simulation::initPopulation() {
    std::cout<<"Initialise Population" << std::endl;
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
        std::cout << "N of free particles is " << params.N << std::endl;
        std::cout << "Total N of particles is " << totalSize() << std::endl;
    }
}
// time stepping of the simulation
    void Simulation::move()
    {
        double timeint = timestep*params.dt; // possibly for adding to age...

        auto p = particles.begin();
        std::advance(p, boundarysize);

        while (p != particles.end()) {
            // zero force and number of contact neighbours for each timestep
            (*p)->setForce({0,0});
            (*p)->setZ(0);
            // get neighbours of p out of neighbour list
            std::list<std::shared_ptr<Particle>> neighbours = domain->getNeighbours((*p)->getIndex());

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
        timestep ++;
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
        double timeint = Ndiv*params.dt;
		
		std::cout << "boundary size " << boundarysize << std::endl;
		std::cout << "initial number of particles " << particles.size() << std::endl;

        while (p != particles.end()) {
                // actual time elapsed since last division check
                // compute probabilistic chance at division
                bool divide = population->testDivide((*p)->getType(),(*p)->getZ(),timeint);
                if (divide) {
                    std::cout << "division" << std::endl;
                    // note: setting the new particle to the same radius as the old will
                    // have unintended consequences (selecting for large particles)
                    double radius = params.R * (1 + params.poly * (disr(gen) - 0.5));
                    // also, random orientation for similar reasons
                    double theta = distheta(gen) * 2 * M_PI;

                    std::array<double,2> x =  {(*p)->getPosition()[0] + disx(gen)*params.eps, (*p)->getPosition()[1] + disx(gen)*params.eps};
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

		std::cout << "number of new particles " << newparticles.size() << std::endl;
		std::cout << "number of particles after division " << particles.size() << std::endl;
        // and rebuild the neighbour list
        if (rebuild) domain->makeNeighbourList(particles);
        // Now, separately, we will check for death
        for (auto p = particles.begin() + boundarysize; p != particles.end(); ){
            bool death = population->testDeath((*p)->getType(),timeint);
            // get rid of the particle
            if (death) {p = particles.erase(p); std::cout << "death" << std::endl;}
            // or continue the loop
            else{++p;}
        }
        //do a neighbour list rebuild to get all the indices straightened out again
        domain->makeNeighbourList(particles);
		std::cout << "Number of particles after division and death " << particles.size() << std::endl;
    }

// function to remove particle in simulation (using id -> idx map in domain)
void Simulation::removeParticle(int i){
    int idx = domain->getIdx(i);
    if (idx == -1){std::cout << "No particle with that id detected" << std::endl;}
    else {
        particles.erase(particles.begin() + idx);
        domain->makeNeighbourList(particles);
    }
}

// function to access particle in particle list : python 
std::shared_ptr<Particle> Simulation::getParticle(int idx){
    return particles[idx];
}

// function to access particle in particle list (using id -> idx map in domain) : python 
std::shared_ptr<Particle> Simulation::getParticlebyId(int id){
    int idx = domain->getIdx(i);
    std::cout << idx << std::endl;
    return particles[idx];
}

// return the population positions
std::vector<std::array<double,2>> Simulation::getPopulationPosition(std::list<int> &index){

    std::vector<std::array<double,2>> positions;
    for (auto i : index) {
        Particle p = *particles[i];
        positions.push_back(p.getPosition());
    }
    return positions;
}

// return the population Ids
std::vector<std::array<double,2>>  Simulation::getPopulationId(std::list<int> &index){

    std::vector<int> ids;
    for (auto i : index) {
        Particle p = *particles[i];
        ids.push_back(p.getId());
    }
    return ids;
}

// return the boundary positions
std::vector<std::array<double,2>> Simulation::getBoundaryPosition(){

    std::vector<std::array<double,2>> positions;
    for (int n = 0; n< boundarysize; ++n) {
        Particle p = *particles[n];
        positions.push_back(p.getPosition());
    }
    return positions;
}

// return the boundary positions
std::vector<std::array<double,2>> Simulation::getBoundaryId(){

    std::vector<int> ids;
    for (int n = 0; n< boundarysize; ++n) {
        Particle p = *particles[n];
        ids.push_back(p.getPosition());
    }
    return ids;
}

std::vector<double> Simulation::getPopulationRadius(std::list<int> &index){

    std::vector<double> radii(0);
    for (auto i : index) {
        Particle p = *particles[i];
        radii.push_back(p.getRadius());
    }
    return radii;
}

void Simulation::updateOutput(){
    output->update(params, boundarysize, particles);
}

void Simulation::saveData(std::string outtype) {
	 // the shared pointer in output is *not* updated routinely
	 output->update(params, boundarysize, particles);
	 if (outtype.compare("vtp") == 0) {
		 output->vtp(timestep);
	 }
	 else if (outtype.compare("text") == 0) {
		 output->savePopulation(timestep);
	 }
	 else {
		 cout << "Error: Unknown output type, doing nothing!";
	 }
}

void Simulation::loadPopulation(std::string filepath)
{
    ifstream inFile;
    inFile.open(filepath);
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }
    int N;
    inFile >> N;
    std::cout << N << std::endl;
    std::string line;

    while (std::getline(inFile, line)) {
        if (line.size() > 0) {
            std::shared_ptr<Particle> pntrP(new Particle(line));
            particles.push_back(std::move(pntrP));
            currentflag += 1;
        }
    }
    inFile.close();
}



