
//
#ifndef CAPMD_POPULATION_H
#define CAPMD_POPULATION_H

#include <vector>
#include <random>

class Population{

public:

        // Constructors
        Population();

        // Additional methods
        void particleDeath(void);

        void particleDivision(void);

        std::vector<std::vector<double>> getPopulationPosition(const std::vector<int> &);

};


#endif //CAPMD_POPULATION_H