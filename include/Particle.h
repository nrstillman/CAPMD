//
//
#ifndef CAPMD_PARTICLE_H
#define CAPMD_PARTICLE_H

#include <vector>

class Particle
    {
    private:
        int id;

    public:
        double radius;
        int age;
        int type;
        std::vector<double> position;
        double theta;
        std::vector<double> force;

        // assign default arguments to constructor
        Particle(int pid = 0, int ptype = 0, std::vector<double> px = {0,0},double ptheta = 0.,double  pr = 1.);
        void initParticle(int,int, std::vector<double>, double, double);

        // additional methods
        int getId() const { return id; }; //Accessor
        void setId( int x) const {const int id = x;} // Mutator

        int getAge() { return age; }
        void setAge( int x) const {int age = x;}

        double getRadius() { return radius; }
        void setRadius( double x) const {double radius = x;}

        std::vector<double> getPosition() { return position; }
        void setPosition(std::vector<double> x) const { std::vector<double> position = x; }

        std::vector<double> getForce() { return force; }
        void setForce(std::vector<double> x) { std::vector<double> force = x; }

        friend std::ostream& operator<< (std::ostream &, const Particle &);
        friend std::istream& operator>> (std::istream &, const Particle &);
    };


#endif //CAPMD_PARTICLE_H
