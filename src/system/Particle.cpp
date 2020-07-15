// Created by N.R. Stillman & S. Henkes 2020
//
#include <Particle.h>

// Particle constructor
Particle::Particle(int pid, int ptype, std::vector<double> px, double ptheta, double pr)
{
    age = 0;
    force = {0,0};
    id = pid;
    type = ptype;
    position = px;
    theta = ptheta;
    radius = pr;
    numneigh = 0;
    z = 0;

//    std::cout << "Particle " << pid <<" Initialised" << std::endl;
}

Particle::Particle(const Particle & rhs)
{
    this->setId(rhs.getId());
    std::cout << "particle copied " << std::endl;
    age = rhs.age;
    force = rhs.force;
    type = rhs.type;
    position = rhs.position;
    theta = rhs.theta;
    radius = rhs.radius;
    numneigh = rhs.numneigh;
    z = rhs.z;
}

// Particle constructor
Particle::Particle(std::string line)
{
    std::vector<double> properties;
    Particle::split(line, '\t', properties);

    if (properties.size() == 0){std::cout << "Particle made with empty string";}

    id = properties.at(0);
    type = properties.at(1);
    age = properties.at(2);
    position = {properties.at(3), properties.at(4)};
    theta = properties.at(5);
    radius = properties.at(6);
    numneigh = 0;
    z = 0;
}

//Particle::~Particle(){
////    delete this;
//}

void Particle::addZ(int i){
    z += i;
}

void Particle::addForce(std::vector<double> f){
    force[0] += f[0];
    force[1] += f[1];
}
std::ostream& operator<<(std::ostream& out,const Particle& p)
{
    return out << p.getId() << '\t' << p.type << '\t' << p.age << '\t' << std::setprecision(4)
                << p.position[0]<< '\t' << std::setprecision(4) << p.position[1] <<  '\t' << std::setprecision(4)
                << p.theta << '\t' << std::setprecision(4) << p.radius << std::endl;
}

// taken from http://stackoverflow.com/a/236803/248823
void Particle::split(const std::string &s, char delim, std::vector<double> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(stod(item));
    }
}
