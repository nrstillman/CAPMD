
//
#include <iostream>
#include <fstream>
#include <Particle.h>
#include <iomanip>      // std::setprecision

// Particle constructor
Particle::Particle(int pid, int ptype, std::vector<double> px, double ptheta, double pr)
{
    initParticle(pid, ptype, px, ptheta, pr);
}

// Particle member functions
void Particle::initParticle(int pid, int ptype, std::vector<double> px, double ptheta, double pr)
{
    age = 0;
    force = {0,0};
    id = pid;
    type = ptype;
    position = px;
    theta = ptheta;
    radius = pr;
}

std::ostream& operator<<(std::ostream& out,const Particle& p)
{
    return out << p.getId() << '\t' << p.type << '\t' << p.age << '\t' << std::setprecision(4)
                << p.position[0]<< '\t' << std::setprecision(4) << p.position[1] <<  '\t' << std::setprecision(4)
                << p.theta << '\t' << std::setprecision(4) << p.radius << std::endl;
}

std::istream& operator>>(std::istream &in, const Particle &p)
{
    int id;
    std::vector<std::string> tmpProperties;
    std::string token;

//    in >> id;//>> id >> p.age >> p.position[0] >> p.position[1];
//    std:: cout << id;
//
//    p.setId(id);
    return in;
}
