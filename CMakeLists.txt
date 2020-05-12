# Will rewrite this clunky cmake file later on

cmake_minimum_required(VERSION 3.13)  # CMake version check
project(capmd)                        # Create project "capmd"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
file(GLOB SOURCE_FILES src/main.cpp src/system/*.cpp include/*.h )

# Add additonal directories
include_directories(src/system)
include_directories(include)

add_subdirectory(pybind11 [../pybind11])
pybind11_add_module(capmd src/main.cpp src/system/Simulation.cpp src/system/Particle.cpp src/system/Population.cpp src/system/Domain.cpp src/system/Interaction.cpp src/system/Dynamics.cpp src/bindings.cpp include/Simulation.h include/Particle.h include/Population.h include/Domain.h include/Dynamics.h include/Interaction.h)

# Add executable target with source files listed in SOURCE_FILES variable
#add_executable(capmd ${SOURCE_FILES})