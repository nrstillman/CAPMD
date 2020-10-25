//
// Created by Namid Stillman on 10/25/20.
//
#include "Interaction.h"
#include "newPotential.h"

Interaction::Interaction(){};

std::shared_ptr<Potential> Interaction::createPotential(Parameters params) {
    switch (params.potential) {

        case 1:
            std::cout << "Chosen new potentials" << std::endl;
            return std::make_shared<newPotential>(params);
//
//        case vanDerWaals:
//            return std::make_unique<vdwPotential>();

        default:
            std::cout << "Chosen old potentials" << std::endl;
            return std::make_shared<Potential>(params);

    }
}
