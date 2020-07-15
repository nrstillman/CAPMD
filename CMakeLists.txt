cmake_minimum_required(VERSION 3.13)  # CMake version check
project(capmd)                        # Create project "capmd"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_library(
    capmd
    STATIC
    ${CMAKE_SOURCE_DIR}/src/system/Simulation.cpp
    ${CMAKE_SOURCE_DIR}/src/system/Particle.cpp
    ${CMAKE_SOURCE_DIR}/src/system/Population.cpp
    ${CMAKE_SOURCE_DIR}/src/system/Domain.cpp
    ${CMAKE_SOURCE_DIR}/src/system/Interaction.cpp
    ${CMAKE_SOURCE_DIR}/src/system/Dynamics.cpp
    ${CMAKE_SOURCE_DIR}/src/io/Output.cpp
)

# Add additonal directories.
include_directories(src/system)
include_directories(src/io)
include_directories(src/bindings)
include_directories(include/CAPMD)

# Add the testing subdir.
add_subdirectory(tests)

# Add executable target for library demo.
add_executable(
    capmd_demo
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)
# Link the CAPMD demo against libcapmd and the VTK libraries.
target_link_libraries(
        capmd_demo
        capmd
        ${VTK_LIBRARIES}
)
# Add pybind11 folder and make python module (called pycapmd)
add_subdirectory(libs/pybind11 ${CMAKE_SOURCE_DIR}/libs/pybind11)
pybind11_add_module(
    pycapmd
    ${CMAKE_SOURCE_DIR}/src/bindings/bindings.cpp
)

# Link vtk libraries to python module
target_link_libraries(
        pycapmd PRIVATE
        capmd
        ${VTK_LIBRARIES}
)

