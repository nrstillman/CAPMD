#include "Simulation.h"

#define _USE_MATH_DEFINES

Simulation::Simulation(){

    std::cout<<"Simulation initialised" << std::endl;
}

Simulation::Simulation(Parameters new_params){

    params = new_params;
}

void Simulation::setParams(Parameters new_params){

    params = new_params;
}

// initialise the system, including creating the particle vector and the first NeighbourList
void Simulation::initialise() {

    // check previous initialisaiton
    if (currentflag == 1){
        particles.clear();
        neighbours.clear();
    } // check if population is empty
    else if (params.N == 0){
        throw "No particles (N=0).";
    }


    // create population of particles
    Simulation::initPopulation();
    // create the first neighbour list
    domain.makeNeighbourList(particles, params.cutoff);

    neighbours = domain.NeighbourList;
    currentflag = 1;
}


void Simulation::initPopulation() {
    if (params.init_opt == "random_unif") {

        std::random_device rd;
        std::mt19937 gen(rd());
        gen.seed(params.initseed);

        // default constructs N Particle objects into the vector
        std::vector<Particle> tmp_vec;

        // RNG: draw from random initial conditions in [-L/2,L/2]x[-L/2,L/2] with random angles
        std::uniform_real_distribution<> disx(0.0, 1);
        std::uniform_real_distribution<> disy(0.0, 1);
        std::uniform_real_distribution<> disr(0.0, 1);
        std::uniform_real_distribution<> distheta(0.0, 1);

        for (int i = 0; i < params.N; i++) {

            double x = (disx(gen) - 0.5) * params.Lx;
            double y = (disy(gen) - 0.5) * params.Ly;
            double theta = distheta(gen) * 2 * M_PI;

            // radius chosen from a uniform distribution with mean R and polydispersity poly
            double radius = params.R * (1 + params.poly * (disr(gen) - 0.5));

            int ptype;
            // assign types here, e.g. 0 is TA cell, 1 is stem cell, 2 is tracer
            if (i < params.NTA) {
                ptype = 1;
            } else if (i < (params.NTA + params.Nstem)) {
                ptype = 2;
            } else {
                ptype = 0;
            }

            Particle newP(i, ptype, {x, y}, theta, radius);
            tmp_vec.push_back(newP);
        }
        particles = tmp_vec;
    }
    if (params.init_opt == "input") {
//       TODO: Add in input from .txt and python
        throw "Cannot accept input yet";
    }
}
    // time stepping of the simulation
    void Simulation::move(void)
    {
        // compute forces
        for (Particle& p : particles) {
            // get neighbours of p out of neighbour list
            std::list<int> neighbours = Simulation::getNeighbours(p);

            for (auto n : neighbours) {
                Particle pj = Simulation::getParticle(n);
                // use interaction to compute force
                std::vector<double> interact = interaction.computeForce(p,pj,domain);
                // add to previous force (ie force += interact)
                std::transform (interact.begin(), interact.end(), p.force.begin(), p.force.begin(), std::plus<double>());
            }
        }
        // using the forces, update the positions and angles
        for (Particle& p : particles) {
            dynamics.step(p, params.dt);
        }
        bool rebuild = domain.checkRebuild(particles, params.maxmove);

        if (rebuild){
                    domain.makeNeighbourList(particles, params.cutoff);
                    neighbours = domain.NeighbourList;
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
        domain.makeNeighbourList(particles, params.cutoff);

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
        // TODO: check this
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
        domain.makeNeighbourList(particles, params.cutoff);
        neighbours = domain.NeighbourList;
    }


// Particle member functions
Particle Simulation::getParticle(int i){
    if (i >= particles.size()){
        std::cout << "Error: index exceeds population size"  << std::endl;
        throw;
    }
    return particles[i];
}

// return the list of neighbours of particle i
std::list<int> Simulation::getNeighbours(Particle p) {
    return neighbours[p.getId()];
}

// return the population positions
std::vector<std::vector<double>> Simulation::getPopulationPosition(std::list<int> &index){

    std::vector<std::vector<double>> positions(0 , std::vector<double>(2));
    for (auto i : index) {
        Particle p = particles[i];
        positions.push_back(p.getPosition());
    }
    return positions;
}

std::vector<double> Simulation::getPopulationRadius(std::list<int> &index){

    std::vector<double> radii(0);
    for (auto i : index) {
        Particle p = particles[i];
        radii.push_back(p.getRadius());
    }
    return radii;
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

//TODO: Check this
void Simulation::loadPopulation(std::string filepath)
{
    std::cout << "offline" <<std::endl;
//    std::string line;
//    std::ifstream in(filepath);
//    getline(in,line);
//    std::cout << line;
//    int i = 0;
//    while(getline(in, l
//    {
//        std::stringstream linestream;
//        std::string data;
//
//        std::getline(linestream, data, '\t');
//        Particle temp;
//        linestream >> temp;
//        ParticleList[i] = temp;
//        i += 1;
//
//    }
}


