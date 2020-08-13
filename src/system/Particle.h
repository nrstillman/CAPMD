// Created by N.R. Stillman & S. Henkes 2020
//
#ifndef CAPMD_PARTICLE_H
#define CAPMD_PARTICLE_H

#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision
#include <iterator>     // for reading txt
#include <sstream>      // for reading txt
#include <algorithm>	// for 'transform' to generate iterators
#include <memory> 		// for shared pointers


class Particle
    {
    private:
        int id;
        int idx;
        std::array<double,2> prevposition;
        int numneigh;
        double z; // must be double for division check later on

    public:
        double radius;
        double age;
        int type;
        std::array<double,2> position;
        double theta;
        std::array<double,2> force;
        std::array<double,2> activeforce;

        // default constructor
        Particle(int pid = 0, int ptype = 0, std::array<double,2> px = {0,0},double ptheta = 0.,double  pr = 1.);

        Particle(const Particle &); // copy constructor

        Particle(std::string); // copy constructor
//
//        ~Particle(); // destructor

        // additional methods
        int getId() const { return id; }; //Accessor
        void setId( int x) {std::cout << "attempted id change!"<< std::endl;} // Mutator

        int getIndex() const { return idx; }; //Accessor
        void setIndex( int x) {idx = x;} // Mutator

        int getType() const { return type; };
        void setType( int x) { type = x;}

        double getAge() { return age;}
        void setAge( double x) { age = x;}

        double getRadius() { return radius;}
        void setRadius( double x) { radius = x;}

        double getTheta() { return theta;}
        void setTheta( double x) { theta = x;}

        int getNumNeigh() { return numneigh;}
        void setNumNeigh( int x) { numneigh= x;}

        int getZ() {return z;}
        void setZ( int x) { z= x;}
        void addZ(int);

        std::array<double,2> getPosition() { return position;}
        void setPosition(std::array<double,2> x) { position = x;}

        std::array<double,2> getPrevPosition() { return prevposition;}
        void setPrevPosition() { prevposition = position;}

        std::array<double,2> getForce() { return force;}
        void setForce(std::array<double,2> x)  { force = x;}
        void addForce(std::array<double,2>);

        void setActiveForce(std::array<double,2> x)  { activeforce = x;}
        void addActiveForce(std::array<double,2>);

        friend std::ostream& operator<< (std::ostream &, const Particle &);
        friend std::istream& operator>> (std::istream &, const Particle &);

        static void split(const std::string &s, char delim, std::vector<double> &elems);
};


#endif //CAPMD_PARTICLE_H