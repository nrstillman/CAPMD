// Python Bindings for CAPMD
// Created by N.R. Stillman & S. Henkes 2020

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

#include <vector>
#include <random>
#include <string>

#include "Particle.h"
#include "Simulation.h"
#include "Parameters.h"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<int>);

PYBIND11_MODULE(pycapmd, m) {
//    //Main simulation happens here
    py::class_<Simulation>(m, "simulation")
        .def(py::init<Parameters>())
        //.def_readwrite("N", &Simulation::N) // parameters to be set in python
        //        .def_readwrite("cutoffZ", &Simulation::cutoffZ)
        //        .def_readwrite("dt", &Simulation::dt)
        //        .def_readwrite("tau", &Simulation::tau)
        .def("initPopulation", &Simulation::initPopulation)
        .def("initBoundary", &Simulation::initBoundary)
        .def("move", &Simulation::move)
        .def("popSize", &Simulation::popSize)
        .def("getBoundarySize", &Simulation::getBoundarySize)
        .def("getParticle", &Simulation::getParticle)
        .def("getParticlebyId", &Simulation::getParticlebyId)
        .def("getPopulationPosition", &Simulation::getPopulationPosition)
        .def("getPopulationId", &Simulation::getPopulationId)
        .def("getBoundaryPosition", &Simulation::getBoundaryPosition)
        .def("getBoundaryId", &Simulation::getBoundaryId)
        .def("getPopulationRadius", &Simulation::getPopulationRadius)
        .def("populationDynamics", &Simulation::populationDynamics)
        .def("initialise", &Simulation::initialise)
		.def("saveData",&Simulation::saveData);
        //.def_readwrite("setParams", &Simulation::setParams);
    
    //Additional info for specific particles (useful for tracking and targeting particles)
    py::class_<Particle>(m, "particle")
        .def(py::init<int, int, std::array<double, 2>, double, double>())
        .def("setId", &Particle::setId)
        .def("getId", &Particle::getId)
        .def("getPosition", &Particle::getPosition)
        .def("getAge", &Particle::getAge);

    //Allow for changing parameters in python scripts
    py::class_<Parameters>(m, "Parameters")
        .def(py::init<>())
        // Domain
        .def_readwrite("Lx", &Parameters::Lx)
        .def_readwrite("Ly", &Parameters::Ly)
        // Simulation
        .def_readwrite("cutoff", &Parameters::cutoff)
        .def_readwrite("dt", &Parameters::dt)
        .def_readwrite("dim", &Parameters::dim)
        .def_readwrite("maxmove", &Parameters::maxmove)
        .def_readwrite("eps", &Parameters::eps)
        // Population
        .def_readwrite("N", &Parameters::N)
        .def_readwrite("init_opt", &Parameters::init_opt)
        .def_readwrite("bc_opt", &Parameters::bc_opt)
        .def_readwrite("NTA", &Parameters::NTA)
        .def_readwrite("Nstem", &Parameters::Nstem)
        .def_readwrite("btype", &Parameters::btype)
        // Particle
        .def_readwrite("flag", &Parameters::flag)
        .def_readwrite("type", &Parameters::type)
        .def_readwrite("R", &Parameters::R)
        .def_readwrite("poly", &Parameters::poly)
        // Interactions
        .def_readwrite("ntypes", &Parameters::ntypes)
        .def_readwrite("fade", &Parameters::fade)
        .def_readwrite("pairstiff", &Parameters::pairstiff)     //pairstiff - nxn mat
        .def_readwrite("pairatt", &Parameters::pairatt)         //pairatt - nxn mat
        // Dyanmics
        .def_readwrite("factive", &Parameters::factive)
        .def_readwrite("zeta", &Parameters::zeta)
        .def_readwrite("tau", &Parameters::tau)
        // Div/Death
        .def_readwrite("deathrate", &Parameters::deathrate)
        .def_readwrite("divrate", &Parameters::divrate)
        .def_readwrite("cutoffZ", &Parameters::cutoffZ)
        .def_readwrite("maxZ", &Parameters::maxZ);

    // Used to convert lists in python to int vectors in c++ (specifically for indexing)
    py::bind_vector<std::vector<int>>(m, "VectorInt");

}
