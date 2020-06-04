add_executable(simulation_tests simulation_tests.cpp)

find_library(PARTICLE_LIBRARY particle_library lib)
find_library(SIMULATION_LIBRARY simulation_library lib)

#target_link_libraries(simulation_tests LINK_PUBLIC ${PARTICLE_LIBRARY})
target_link_libraries(simulation_tests LINK_PUBLIC ${SIMULATION_LIBRARY})
