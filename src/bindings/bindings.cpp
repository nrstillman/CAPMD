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
        .def(py::init<>())
        .def_readwrite("N", &Simulation::N) // parameters to be set in python
        .def_readwrite("cutoffZ", &Simulation::cutoffZ)
        .def_readwrite("dt", &Simulation::dt)
        .def_readwrite("tau", &Simulation::tau)
        .def("initPopulation", &Simulation::initPopulation)
        .def("initBoundary", &Simulation::initBoundary)
        .def("move", &Simulation::move)
        .def("popSize", &Simulation::popSize)
        .def("getBoundarySize", &Simulation::getBoundarySize)
        .def("getParticle", &Simulation::getParticle)
        .def("getPopulationPosition", &Simulation::getPopulationPosition)
        .def("getBoundaryPosition", &Simulation::getBoundaryPosition)
        .def("getPopulationRadius", &Simulation::getPopulationRadius);

    //Additional info for specific particles (useful for tracking and targeting particles)
    py::class_<Particle>(m, "particle")
            .def(py::init<int, int, std::vector<double>, double, double>())
            .def("setId", &Particle::setId)
            .def("getId", &Particle::getId)
            .def("getPosition", &Particle::getPosition)
            .def("getAge", &Particle::getAge);

}

